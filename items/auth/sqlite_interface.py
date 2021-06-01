'''
Copyright 2014-2021 Integrated Test Management Suite

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
'''
from hashlib import sha256
import os
from sqlite3.dbapi2 import Cursor
from typing import Tuple, Union
from uuid import uuid4
import sqlite3
import logging
from account_status import AccountStatus
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING
from logon_type import LogonType

class SqliteInterfaceException(Exception):
    """ Sqlite interface class base exception. """

class SqliteInterface:
    """ Sqlite database wrapper """
    __slots__ = ['_connection', '_database_filename', '_logger',
                 '_is_connected']

    sql_create_user_profile_table = """
        CREATE TABLE IF NOT EXISTS user_profile (
            id integer PRIMARY KEY,
            email_address text NOT NULL,
            full_name text NOT NULL,
            display_name text NOT NULL,
            insertion_date integer NOT NULL,
            account_status integer DEFAULT 0,
            logon_type integer DEFAULT 0 NOT NULL
        )
    """

    default_admin_user = {
        'email_address' : 'admin@localhost',
        'full_name' : 'Local Admin',
        'display_name' : 'Local Admin',
        'account_status' : AccountStatus.ACTIVE.value,
        'logon_type': LogonType.BASIC.value
    }

    sql_create_user_auth_details_table = """
        CREATE TABLE IF NOT EXISTS user_auth_details (
            id integer PRIMARY KEY,
            password text NOT NULL,
            password_salt text NOT NULL,
            user_id integer NOT NULL,

            FOREIGN KEY(user_id) REFERENCES user_profile(id)
        )
    """

    @property
    def is_connected(self) -> bool:
        """!@brief Is connected (Getter).

        returns:
            Boolean if connected or not.
        """
        return self._is_connected and self._connection is not None

    def __init__(self, database_filename : str) -> None:
        """
        Class constructor.

        parameters:
            database_filename Filename and path of the database.

        returns:
            SqliteInterface instance.
        """

        self._connection = None
        self._database_filename = database_filename
        self._is_connected = False

        self._logger = logging.getLogger(__name__)
        log_format= logging.Formatter(LOGGING_LOG_FORMAT_STRING,
                                      LOGGING_DATETIME_FORMAT_STRING)
        console_stream = logging.StreamHandler()
        console_stream.setFormatter(log_format)
        self._logger.setLevel(LOGGING_DEFAULT_LOG_LEVEL)
        self._logger.addHandler(console_stream)

    def valid_database(self) -> bool:
        """
        Check to see if the database is valid.  We verify that:
          a) The file exist.
          b) It's at least 100 bytes (min size of header).
          c) Contains the expected header string.

        returns:
            True if valid, False = not valid.
        """

        status = False

        if os.path.isfile(self._database_filename):

            # SQLite database file header is 100 bytes
            if os.path.getsize(self._database_filename) > 100:

                with open(self._database_filename, 'rb') as handle:
                    header = handle.read(100)

                    if header[:16] == b'SQLite format 3\x00':
                        status = True

        return status

    def build_database(self) -> Tuple[bool, str]:
        """
        Build a new, empty database ready for use. The build process closes the
        the database at the end.

        returns:
             Tuple of [status, status_str]
             status_str value is empty, unless the status is False (failed)
        """

        build_status = False
        build_err_str = ''

        if not os.path.isfile(self._database_filename):
            self._logger.info("Building database...")

            try:
                self._connection = sqlite3.connect(self._database_filename)

                cursor = self._connection.cursor()

                self._logger.info("-> Creating user_profile table")
                self._create_table(cursor, self.sql_create_user_profile_table,
                                   'user_profile')

                self._logger.info("-> Creating user_auth_details table")
                self._create_table(cursor, self.sql_create_user_auth_details_table,
                                   'user_auth_details')

                admin_password = str(uuid4())[:13]
                admin_password_salt = str(uuid4())

                self._logger.info("-> Creating admin with password '%s'",
                                  admin_password)

                user_id = self._insert_user_profile(
                    cursor,
                    self.default_admin_user.get('email_address'),
                    self.default_admin_user.get('full_name'),
                    self.default_admin_user.get('display_name'),
                    self.default_admin_user.get('account_status'),
                    self.default_admin_user.get('logon_type')
                )

                self._insert_basic_auth_entry(cursor, user_id, admin_password,
                                              admin_password_salt)

                self._logger.info("Database build successful")
                build_status = True

            except SqliteInterfaceException as interface_except:
                build_err_str = str(interface_except)

            except sqlite3.OperationalError as op_except:
                return (False, f'Database build failed: {str(op_except)}')

            cursor.close()
            self._connection.close()

        else:
            build_err_str = ("Database with filename "
                            f"'{self._database_filename}' already exists!")

        return (build_status, build_err_str)

    def open(self) -> Tuple[bool, str]:
        """
        Open a database and store connection for later use.

        returns:
            Boolean indicating success status.
        """

        open_status = False
        err_str = ''

        try:
            self._connection = sqlite3.connect(self._database_filename)
            open_status = True
            self._is_connected = True

        except sqlite3.Error as sqlite_except:
            err_str = f'open failed, reason: {sqlite_except}'
            self._connection = None

        return (open_status, err_str)

    def close(self) -> None:
        """
        Close the connection.

        returns:
            None.
        """

        if self._connection:
            self._connection.close()

        self._connection = None

    def valid_user_to_logon(self, email_address : str, logon_type : int) \
                            -> Tuple[Union[int, None], str]:
        """
        Check to see if a user is able to logon based on email address and the
        logon type.

        parameters:
            email_address - User's email address\n
            logon_type - Logon type
        """

        user_id = None
        error_str = ''

        query = ("SELECT id, logon_type, account_status FROM user_profile "
                 "WHERE email_address = ?")

        cursor = self._connection.cursor()

        try:
            cursor.execute(query, (email_address,))

        except sqlite3.Error as sqlite_except:
            raise RuntimeError(f'Query failed, reason: {sqlite_except}') from \
                sqlite_except

        rows = cursor.fetchall()

        if rows:
            user_id = rows[0][0]
            account_logon_type = rows[0][1]
            acocunt_status = rows[0][2]

            if account_logon_type != logon_type:
                error_str = 'Incorrect logon type'

            elif acocunt_status != AccountStatus.ACTIVE.value:
                error_str = 'Account not active'

        else:
            error_str = 'Unknown e-mail address'

        return (user_id, error_str)

    def basic_user_authenticate(self, user_id : int, password : str) \
                                -> Tuple[Union[int, None], str]:
        """
        Authenticate a user using basic authentication (email address and a
        password.

        parameters:
            user_id - User's unique id\n
            password - Password to authenticate
        """

        return_status = False
        return_status_str = ''

        cursor = self._connection.cursor()

        try:
            query = ("SELECT password, password_salt FROM user_auth_details "
                     "WHERE user_id = ?")
            cursor.execute(query, (user_id,))

        except sqlite3.Error as sqlite_except:
            raise SqliteInterfaceException(
                f'Query failed, reason: {sqlite_except}') from sqlite_except

        rows = cursor.fetchall()

        if not rows:
            raise SqliteInterfaceException('Invalid user id')

        recv_password = rows[0][0]
        recv_password_salt = rows[0][1]

        to_hash = f"{password}{recv_password_salt}".encode('UTF-8')
        password_hash = sha256(to_hash).hexdigest()

        if password_hash != recv_password:
            return_status_str = "Username/password don't match"

        else:
            return_status = True

        return (return_status, return_status_str)

    def _create_table(self, cursor : Cursor, table_schema : str,
                      table_name : str) -> None:
        """
        Create a new database table.

        parameters:
            cursor - Sqlite database cursor object\n
            table_schema - Schema of table to create\n
            table_name - name of table within Sqlite\n

        returns:
            None.
        """
        # pylint: disable=R0201

        try:
            cursor.execute(table_schema)

        except sqlite3.Error as mysqlite_exception:
            raise SqliteInterfaceException(
                (f"Failed to create table '{table_name}', reason: "
                 f"{str(mysqlite_exception)}")) from mysqlite_exception

    def _insert_user_profile(self, cursor : Cursor, email_address : str,
                             full_name : str, display_name : str,
                             account_status : int, logon_type : int) -> int:
        '''
        Insert a user profile into the database.

        parameters:
            cursor - Sqlite database cursor\n
            email_address - User's email address\n
            full_name - Full name of the user (e.g. first name, surname)\n
            display_name - Name to display\n
            account_status - Status code of account (e.g. active)\n
            logon_type - Type of login, such as basic

        returns:
            int - new user_id
        '''
        # pylint: disable=R0913

        query = ("INSERT INTO user_profile (email_address, full_name, "
                 "display_name, insertion_date, account_status, logon_type) "
                 f"VALUES('{email_address}', '{full_name}', '{display_name}', "
                 f"0, {account_status}, {logon_type})")

        try:
            cursor.execute(query)
            self._connection.commit()
            new_user_id = cursor.lastrowid

        except sqlite3.Error as sqlite_except:
            raise SqliteInterfaceException(
                f'Query failed, reason: {sqlite_except}') from sqlite_except

        return new_user_id

    def _insert_basic_auth_entry(self, cursor : Cursor, user_id : int,
                                 password : str, password_salt : str) -> None:

        '''
        Insert a user authentication entry into the database.

        parameters:
            cursor - Sqlite database cursor\n
            user_id - Id of user associated with entry\n
            password - Password\n
            password_salt - Random password salt

        returns:
            None
        '''

        to_hash = f"{password}{password_salt}".encode('UTF-8')
        password_hash = sha256(to_hash).hexdigest()

        query = ("INSERT INTO user_auth_details (password, password_salt, "
                 "user_id) "
                 f"VALUES('{password_hash}', '{password_salt}', {user_id})")

        try:
            cursor.execute(query)
            self._connection.commit()

        except sqlite3.Error as sqlite_except:
            raise SqliteInterfaceException(
                f'Query failed, reason: {sqlite_except}') from sqlite_except
