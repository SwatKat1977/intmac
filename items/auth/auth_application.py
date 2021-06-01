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
import logging
import os
from application import Application
from config import Config
from sqlite_interface import SqliteInterface
from views.basic_auth_view import create_basic_auth_blueprint
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING

class AuthApplication(Application):
    """ ITEMS Authentication Service """
    __slots__ = ["_config", "_db", "_logger"]

    def __init__(self, quart_instance):
        super().__init__()
        self._quart_instance = quart_instance
        self._config = None
        self._db = None

        self._logger = logging.getLogger(__name__)
        log_format= logging.Formatter(LOGGING_LOG_FORMAT_STRING,
                                      LOGGING_DATETIME_FORMAT_STRING)
        console_stream = logging.StreamHandler()
        console_stream.setFormatter(log_format)
        self._logger.setLevel(LOGGING_DEFAULT_LOG_LEVEL)
        self._logger.addHandler(console_stream)

    def _initialise(self) -> bool:

        return_status = False

        config_mgr = Config()
        self._config = config_mgr.read_config("./config.ini")

        if self._check_database() and self._open_database():
            basic_auth_blueprint = create_basic_auth_blueprint(self._db)
            self._quart_instance.register_blueprint(basic_auth_blueprint)

            return_status = True

        return return_status

    def _main_loop(self) -> None:
        ''' Abstract method for main application. '''

    def _shutdown(self):
        ''' Abstract method for application shutdown. '''

    def _open_database(self):

        self._db = SqliteInterface('self._config.database.database_file')

        status = False

        open_status, err_str = self._db.open()
        if not open_status:
            self._logger.critical(err_str)

        else:
            status = True
            self._logger.info("Database '%s' opened successful",
                              self._config.database.database_file)

        return status

    def _check_database(self) -> bool:

        db_status = False

        self._db = SqliteInterface(self._config.database.database_file)

        if os.path.isfile(self._config.database.database_file):

            if not self._db.valid_database():
                self._logger.critical("Database file '%s' is not valid!",
                                      self._config.database.database_file)
            else:
                db_status = True

        else:
            if self._config.database.create_when_missing:
                status, err_str = self._db.build_database()
                if not status:
                    self._logger.critical(err_str)

                else:
                    self._logger.info("Database created successfully")
                    db_status = True

            else:
                self._logger.critical(("Database file '%s' doesn't exist and "
                                       "won't get created!"),
                                       self._config.database.database_file)

        return db_status
