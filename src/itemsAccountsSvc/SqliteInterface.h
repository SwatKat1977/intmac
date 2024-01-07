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
-----------------------------------------------------------------------------
*/
#include <exception>
#include <map>
#include <string>
#include <sqlite3.h>

namespace items
{
    namespace accountsSvc
    {
        // Status of the account.
        enum AccountStatus
        {
            AccountStatus_DISABLED = 0,
            AccountStatus_ACTIVE = 1
        };

        // Type of logon, e.g. basic.
        enum LogonType
        {
            LogonType_BASIC = 0
        };

        // Sqlite interface class base exception.
        class SqliteInterfaceException : public std::exception
        {
        public:
            explicit SqliteInterfaceException (const char* message)
                : m_msg (message) {}

            explicit SqliteInterfaceException (const std::string& message)
                : m_msg (message) {}

            virtual const char* what () const noexcept { return m_msg.c_str (); }

        protected:
            std::string m_msg;
        };

        // Sqlite database wrapper class.
        class SqliteInterface
        {
        public:

            SqliteInterface (std::string dbFilename);

            ~SqliteInterface ();

            bool IsConnected () { return (m_isConnected && m_connection); }

            bool IsValidDatabase ();

            void BuildDatabase ();

            void Open ();

            void Close ();

        private:
            sqlite3* m_connection;
            std::string m_dbFilename;
            bool m_isConnected;

            void CreateTable (std::string tableSchema, std::string table_name);

            int InsertUserProfile (std::string emailAddress,
                                   std::string fullName,
                                   std::string displayName,
                                   int accountStatus,
                                   int logonType);

            void InsertBasicAuthEntry (int userId,
                                       std::string password,
                                       std::string passwordSalt);
        };

    }   // namespace accountsSvc
}   // namespace items
