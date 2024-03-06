/*
-----------------------------------------------------------------------------
This source file is part of ITEMS
(Integrated Test Management Suite)
For the latest info, see https://github.com/SwatKat1977/intmac/

Copyright 2014-2023 Integrated Test Management Suite Development Team

    This program is free software : you can redistribute it and /or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see < https://www.gnu.org/licenses/>.

The following is based on Ogre3D code:
* GetEnv from os-int.h
-----------------------------------------------------------------------------
*/
#include <exception>
#include <filesystem>
#include <sstream>
#include "SqliteInterface.h"
#include "Definitions.h"
#include "Logger.h"
#include "Sha256.h"
#include "UUID.h"

namespace items { namespace accountsSvc {

const char* SQLITE_HEADER = "SQLite format 3\x00";

const char CREATE_USER_PROFILE_TABLE[] =
    "CREATE TABLE IF NOT EXISTS user_profile ("
    "id integer PRIMARY KEY,"
    "email_address text NOT NULL,"
    "full_name text NOT NULL,"
    "display_name text NOT NULL,"
    "insertion_date integer NOT NULL,"
    "account_status integer DEFAULT 0,"
    "logon_type integer DEFAULT 0 NOT NULL)";

const char CREATE_USER_AUTH_DETAILS_TABLE[] =
    "CREATE TABLE IF NOT EXISTS user_auth_details ("
    "id integer PRIMARY KEY,"
    "password text NOT NULL,"
    "password_salt text NOT NULL,"
    "user_id integer NOT NULL,"
    "FOREIGN KEY (user_id) REFERENCES user_profile (id)"
    ")";

std::map<std::string, std::string> DEFAULT_ADMIN_USER = {
    { "email_address", "admin@localhost" },
    { "full_name", "Local Admin" },
    { "display_name", "Local Admin" },
    { "account_status", std::to_string(AccountStatus_ACTIVE) },
    { "logon_type", std::to_string(LogonType_BASIC) }
};

const char QUERY_GET_USERID_FOR_LOGIN[] =
    "SELECT id, account_status FROM user_profile "
    "WHERE email_address = ? AND logon_type = ?";

const char QUERY_BASIC_AUTHENTICATE_USER[] =
    "SELECT password, password_salt FROM user_auth_details "
    "WHERE user_id = ?";

SqliteInterface::SqliteInterface(std::string dbFilename)
    : m_connection(nullptr), m_dbFilename(dbFilename),
      m_isConnected(false) {}

SqliteInterface::~SqliteInterface() {
    LOGGER->info("Cleaning up SQLite interface...");
    Close();
}

bool SqliteInterface::IsValidDatabase() {
    bool status = false;

    std::string filepath = { m_dbFilename };

    // Verify if the database file exists and is over 100 bytes as that
    // is the size of the Sqlite database header.
    if (std::filesystem::is_regular_file(filepath) &&
        std::filesystem::file_size(filepath) > 100) {
        char bufferBytes[100] = { 0 };

        FILE* fp = fopen(m_dbFilename.c_str(), "rb");
        if (!fp) return false;

        fread(bufferBytes, 100, 1, fp);

        if (strcmp(SQLITE_HEADER, bufferBytes) == 0) {
            status = true;
        }

        fclose(fp);
    }

    return status;
}

void SqliteInterface::BuildDatabase() {
    std::string filepath = { m_dbFilename };
    if (std::filesystem::is_regular_file(filepath)) {
        throw SqliteInterfaceException(
            "Database '" + m_dbFilename + "' already exists!");
    }

    LOGGER->info("Building database...");

    try {
        int openStatus = sqlite3_open(m_dbFilename.c_str(),
                                      &m_connection);
        if (openStatus) {
            std::string except = "Error opening DB : " +
                std::string(sqlite3_errmsg(m_connection));
            throw SqliteInterfaceException(except);
        }

        LOGGER->info("-> Creating user_profile table");
        CreateTable(CREATE_USER_PROFILE_TABLE, "user_profile");

        LOGGER->info("-> Creating user_auth_details table");
        CreateTable(CREATE_USER_AUTH_DETAILS_TABLE,
            "user_auth_details");

        std::string adminPassword = common::UUID::New().ToString();
        std::string adminPasswordSalt = common::UUID::New().ToString();

        LOGGER->info("-> Creating admin with password '{0}'",
            adminPassword);

        int userId = InsertUserProfile(
            DEFAULT_ADMIN_USER["email_address"],
            DEFAULT_ADMIN_USER["full_name"],
            DEFAULT_ADMIN_USER["display_name"],
            atoi(DEFAULT_ADMIN_USER["account_status"].c_str()),
            atoi(DEFAULT_ADMIN_USER["logon_type"].c_str()));

        InsertBasicAuthEntry(
            userId,
            adminPassword,
            adminPasswordSalt);

        LOGGER->info("Database built successfully...");
    }
    catch (SqliteInterfaceException &interface_except) {
        throw;
    }

    Close();
}

void SqliteInterface::Open() {
    if (m_connection) {
        throw SqliteInterfaceException("Database is already open");
    }

    int openStatus = sqlite3_open(m_dbFilename.c_str(), &m_connection);
    if (openStatus) {
        std::string except = "Error opening DB : " +
            std::string(sqlite3_errmsg(m_connection));
        throw SqliteInterfaceException(except);
    }
}

void SqliteInterface::Close() {
    if (m_connection) {
        sqlite3_close(m_connection);
        m_connection = nullptr;
    }
}

void SqliteInterface::CreateTable(
    std::string tableSchema,
    std::string tableName) {
    char* errMsg = 0;

    int result = sqlite3_exec(m_connection,
        tableSchema.c_str(),
        nullptr,
        0,
        &errMsg);
    if (result != SQLITE_OK) {
        std::string exceptStr = "Create table failure : " +
            std::string(errMsg);
        sqlite3_free(errMsg);
        throw SqliteInterfaceException(exceptStr);
    }
}

int SqliteInterface::InsertUserProfile(std::string emailAddress,
    std::string fullName,
    std::string displayName,
    int accountStatus,
    int logonType) {
    std::stringstream query;
    query << "INSERT INTO user_profile "
        << "(email_address, full_name, display_name, insertion_date,"
        << " account_status, logon_type) "
        << "VALUES("
        << "'" << emailAddress << "',"
        << "'" << fullName << "',"
        << "'" << displayName << "', "
        << "0, "
        << accountStatus << ", "
        << logonType << ")";

    char* errMsg = 0;

    int result = sqlite3_exec(m_connection,
        query.str().c_str(),
        nullptr,
        0,
        &errMsg);
    if (result != SQLITE_OK) {
        std::string exceptStr = "Unable to add user profile : " +
            std::string(errMsg);
        sqlite3_free(errMsg);
        throw SqliteInterfaceException(exceptStr);
    }

    return static_cast<int>(sqlite3_last_insert_rowid(m_connection));
}

void SqliteInterface::InsertBasicAuthEntry(int userId,
    std::string password,
    std::string passwordSalt) {
    std::string toHash = password + passwordSalt;
    std::string passwordHash = GenerateSha256(toHash);

    std::stringstream query;
    query << "INSERT INTO user_auth_details (password, password_salt, "
        << "user_id) "
        << "VALUES('"
        << passwordHash << "', '"
        << passwordSalt << "', "
        << userId << ")";

    char* errMsg = 0;

    int result = sqlite3_exec(m_connection,
        query.str().c_str(),
        nullptr,
        0,
        &errMsg);
    if (result != SQLITE_OK) {
        std::string exceptStr = "Unable to add user auth details : " +
            std::string(errMsg);
        sqlite3_free(errMsg);
        throw SqliteInterfaceException(exceptStr);
    }
}

int SqliteInterface::GetUserIdForUser(std::string emailAddress,
    int logonType) {
    sqlite3_stmt* stmt = 0;

    int prepareStatus = sqlite3_prepare_v2(
        m_connection,
        QUERY_GET_USERID_FOR_LOGIN,
        strlen(QUERY_GET_USERID_FOR_LOGIN),
        &stmt,
        NULL);

    if (prepareStatus != SQLITE_OK) {
        LOGGER->critical("GetUserIdForUser SQL statement threw"
            "error on prepare stage : {0}",
            sqlite3_errmsg(m_connection));
        return -1;
    }

    sqlite3_bind_text(
        stmt,
        1,
        emailAddress.c_str(),
        static_cast<int>(emailAddress.size()),
        SQLITE_STATIC);

    sqlite3_bind_int(stmt, 2, logonType);

    int stepStatus = 0;
    int queryCount = 0;
    int userId = -1;

    while ((stepStatus = sqlite3_step(stmt)) == SQLITE_ROW) {
        queryCount++;

        if (queryCount > 1) {
            throw SqliteInterfaceException("Duplicate entries");
        }

        userId = sqlite3_column_int(stmt, 0);
        int accountStatus = sqlite3_column_int(stmt, 1);

        if (accountStatus != AccountStatus_ACTIVE) {
            throw SqliteInterfaceException("Account not active");
        }
    }

    if (stepStatus != SQLITE_DONE) {
        throw SqliteInterfaceException(
            std::string("SqliteInterface::GetUserIdForUser threw ") +
            + "exception " + sqlite3_errmsg(m_connection));
    }

    sqlite3_finalize(stmt);

    return userId;
}

bool SqliteInterface::BasicAuthenticateUser(int userId,
                                            std::string password) {
    bool status = false;
    sqlite3_stmt* stmt = 0;

    int prepareStatus = sqlite3_prepare_v2(
        m_connection,
        QUERY_BASIC_AUTHENTICATE_USER,
        strlen(QUERY_BASIC_AUTHENTICATE_USER),
        &stmt,
        NULL);

    if (prepareStatus != SQLITE_OK) {
        LOGGER->critical("BasicAuthenticateUser SQL statement threw"
            "error on prepare stage : {0}",
            sqlite3_errmsg(m_connection));
        return false;
    }

    sqlite3_bind_int(stmt, 1, userId);

    int stepStatus = 0;
    int queryCount = 0;

    while ((stepStatus = sqlite3_step(stmt)) == SQLITE_ROW) {
        queryCount++;

        if (queryCount > 1) {
            throw SqliteInterfaceException("Duplicate entries");
        }

        std::string recordPassword = (char *)sqlite3_column_text(   // NOLINT
            stmt, 0);
        std::string recordPasswordSalt = (char *)sqlite3_column_text( // NOLINT
            stmt, 1);

        // Neither should be null, but just in case...
        if (recordPassword.empty() || recordPasswordSalt.empty()) {
            break;
        }

        std::string hashStr = std::string(password) +
            std::string(recordPasswordSalt);
        std::string passwordHash = GenerateSha256(hashStr);

        if (passwordHash == recordPassword) {
            status = true;
        }
    }

    if (stepStatus != SQLITE_DONE) {
        LOGGER->critical(
            "SqliteInterface::BasicAuthenticateUser threw {0}",
            sqlite3_errmsg(m_connection));
        status = false;
    }

    sqlite3_finalize(stmt);

    return status;
}

    }   // namespace accountsSvc
}   // namespace items
