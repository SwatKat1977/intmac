'''
Copyright 2023 Integrated Test Management Suite

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
import sqlite3
from sqlite_client import SqliteClient, SqliteClientException

class SqliteInterface(SqliteClient):

    sql_create_testcase_filter_table = """
        CREATE TABLE IF NOT EXISTS testcase_filter (
            id INT PRIMARY KEY,
            name VARCHAR(100) NOT NULL,
            description TEXT NOT NULL DEFAULT '',
            parent INT NOT NULL
        )
    """

    def build_database(self) -> bool:
        """
        Build a new, empty database ready for use. The build process closes the
        the database at the end.

        returns:
             Boolean status from database build
        """

        build_status : bool = False

        if not os.path.isfile(self._database_filename):
            self._logger.info("Building database...")

            try:
                self._connection = sqlite3.connect(self._database_filename)

                cursor = self._connection.cursor()

                self._logger.info("-> Creating testcase_filter table")
                self._create_table(cursor, self.sql_create_testcase_filter_table,
                                   'testcase_filter')

                self._logger.info("Database build successful")
                build_status = True

            except SqliteClientException as interface_except:
                self._logger.critical("Building database failed, reason: %s",
                                  str(interface_except))

            except sqlite3.OperationalError as op_except:
                self._logger.critical('Database build failed: %s',
                                  str(op_except))

            cursor.close()
            self._connection.close()

        else:
            self._logger.critical('Database with filename %s already exists!',
                                  self._database_filename)

        return build_status
