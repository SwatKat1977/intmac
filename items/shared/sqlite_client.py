'''
Copyright 2014-2023 Integrated Test Management Suite

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
import logging

class SqliteClientException(Exception):
    """ Sqlite client class base exception. """

class SqliteClient:
    """ Sqlite database wrapper """
    __slots__ = ['_connection', '_database_filename', '_logger',
                 '_is_connected']

    @property
    def is_connected(self) -> bool:
        """!@brief Is connected (Getter).

        returns:
            Boolean if connected or not.
        """
        return self._is_connected and self._connection is not None

    def __init__(self, logger : logging.Logger, database_filename : str) -> None:
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
        self._logger = logger.getChild(__name__)

    def __del__(self):
        self._logger.info("Cleaning up SQLite interface...")
        self.close()

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

    def build_database(self) -> bool:
        """
        Build a new, empty database ready for use. The build process closes the
        the database at the end.

        returns:
             Boolean status from database build
        """
        raise NotImplementedError

    def open(self) -> Tuple[bool, str]:
        """
        Open a database and store connection for later use.

        returns:
            Boolean indicating success status.
        """

        open_status = False
        err_str = ''

        try:
            self._connection = sqlite3.connect(self._database_filename,
                                               check_same_thread=False)
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

        try:
            cursor.execute(table_schema)

        except sqlite3.Error as mysqlite_exception:
            raise SqliteClientException(
                (f"Failed to create table '{table_name}', reason: "
                 f"{str(mysqlite_exception)}")) from mysqlite_exception
