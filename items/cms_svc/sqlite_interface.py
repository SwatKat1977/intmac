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

    # Column names for testsuite
    TESTSUITE_COLUMN_ID = 'id'
    TESTSUITE_COLUMN_NAME = 'name'
    TESTSUITE_COLUMN_DESCRIPTION = 'description'
    TESTSUITE_COLUMN_PARENT = 'parent'

    sql_create_project_table = """
        CREATE TABLE IF NOT EXISTS project (
            id INTEGER PRIMARY KEY,
            name VARCHAR(150) NOT NULL,
            description TEXT NOT NULL DEFAULT ''
        )
    """

    sql_create_testcase_table = """
        CREATE TABLE IF NOT EXISTS testcase (
            id INTEGER PRIMARY KEY,
            title VARCHAR(100) NOT NULL,
            testsuite_id INTEGER NOT NULL
        )
    """

    sql_create_testsuite_table = """
        CREATE TABLE IF NOT EXISTS testsuite (
            id INTEGER PRIMARY KEY,
            project_id INTEGER,
            name VARCHAR(100) NOT NULL,
            description TEXT NOT NULL DEFAULT '',
            parent INTEGER NOT NULL
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

                self._logger.info("-> Creating project table")
                self._create_table(cursor, self.sql_create_project_table,
                                   'project')

                self._logger.info("-> Creating testcase table")
                self._create_table(cursor, self.sql_create_testcase_table,
                                   'testcase')

                self._logger.info("-> Creating testsuite table")
                self._create_table(cursor, self.sql_create_testsuite_table,
                                   'testsuite')

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

    def get_testsuites_for_project(self, project_id : int) -> list:
        """
        Get all testsuites for a given project.

        parameters:
            project_id - Project ID for testsuites
        """

        query : str = ("SELECT id, name, description, parent FROM testsuite "
                       "WHERE project_id = ?")

        cursor = self._connection.cursor()

        try:
            cursor.execute(query, (project_id,))

        except sqlite3.Error as sqlite_except:
            raise RuntimeError(f'Query failed, reason: {sqlite_except}') from \
                sqlite_except

        rows = cursor.fetchall()

        testsuites : list = []

        if rows:
            for suite in rows:
                entry = {
                    self.TESTSUITE_COLUMN_ID : suite[0],
                    self.TESTSUITE_COLUMN_NAME : suite[1],
                    self.TESTSUITE_COLUMN_DESCRIPTION : suite[2],
                    self.TESTSUITE_COLUMN_PARENT : suite[3]
                }

                testsuites.append(entry)

        return testsuites

    def is_valid_project_id(self, project_id : int) -> bool:

        query : str = "SELECT id FROM project WHERE id = ?"

        cursor = self._connection.cursor()

        try:
            cursor.execute(query, (project_id,))

        except sqlite3.Error as sqlite_except:
            raise RuntimeError(f'Query failed, reason: {sqlite_except}') from \
                sqlite_except

        rows = cursor.fetchall()

        return True if rows else False
 