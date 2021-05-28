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
import os
from sqlite3.dbapi2 import Cursor
from typing import Tuple
import sqlite3

class SqliteInterfaceException(Exception):
    """ Sqlite interface class base exception. """

class SqliteInterface:
    """ Sqlite database wrapper """
    __slots__ = ['_connection', '_database_filename', '_is_connected']

    sql_create_user_profile_table = """
        CREATE TABLE IF NOT EXISTS user_profile (
            id integer PRIMARY KEY,
            email_address text NOT NULL,
            full_name text NOT NULL,
            display_name text NOT NULL,
            insertion_date integer NOT NULL,
            account_status integer DEFAULT 0
        )
    """

    sql_create_user_auth_details_table = """
        CREATE TABLE IF NOT EXISTS user_auth_details (
            id integer PRIMARY KEY,
            login_type integer DEFAULT 0 NOT NULL,
            password text NOT NULL,
            password_salt text NOT NULL
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

    def database_exists(self) -> bool:
        """
        Check to see if the database exists.  We verify that:
          a) The file exist.
          b) It's at least 100 bytes (min size of header).
          c) Contains the expected header string.

        returns:
            True if valid, False = not valid.
        """

        if not os.path.isfile(self._database_filename):
            return False

        # SQLite database file header is 100 bytes
        if os.path.getsize(self._database_filename) < 100:
            return False

        with open(self._database_filename, 'rb') as handle:
            header = handle.read(100)

        return header[:16] == b'SQLite format 3\x00'

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

        if os.path.isfile(self._database_filename):
            return (False, ("Database with filename "
                            f"'{self._database_filename}'' already exists!"))

        try:
            self._connection = sqlite3.connect(self._database_filename)

        except sqlite3.OperationalError as op_except:
            return (False, f'Database build failed: {str(op_except)}')

        cursor = self._connection.cursor()

        try:
            self._create_table(cursor, self.sql_create_user_profile_table,
                               'user_profile')

            self._create_table(cursor, self.sql_create_user_auth_details_table,
                               'user_auth_details')

            build_status = True

        except SqliteInterfaceException as interface_except:
            build_err_str = str(interface_except)

        cursor.close()
        self._connection.close()
        return (build_status, build_err_str)

    def open(self) -> bool:
        """
        Open a database and store connection for later use.

        returns:
            Boolean indicating success status.
        """

        open_status = False

        try:
            self._connection = sqlite3.connect(self._database_filename)
            cursor = self._connection.cursor()
            cursor.execute('SELECT id FROM url_queue LIMIT 1')
            open_status = True
            self._is_connected = True

        except sqlite3.Error as sqlite_except:
            self._last_error_msg = f'open failed, reason: {sqlite_except}'
            self._connection = None

        return open_status

    def close(self) -> None:
        """
        Close the connection.

        returns:
            None.
        """
        if self._connection:
            self._connection.close()

        self._connection = None

    def _create_table(self, cursor : Cursor, table_schema : str,
                      table_name : str) -> None:
        """
        Create a new database table.

        parameters:
            cursor - Sqlite database cursor object
            table_schema - Schema of table to create
            table_name - name of table within Sqlite.

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
