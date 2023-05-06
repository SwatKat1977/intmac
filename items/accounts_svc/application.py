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
import asyncio
import logging
import os
from base_application import BaseApplication, COPYRIGHT_TEXT, LICENSE_TEXT
from configuration_layout import CONFIGURATION_LAYOUT
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING
from sqlite_interface import SqliteInterface
from threadsafe_configuration import ThreadafeConfiguration
from version import BUILD_TAG, BUILD_VERSION, RELEASE_VERSION
from views.basic_auth_view import create_basic_auth_blueprint

class Application(BaseApplication):
    """ ITEMS Accounts Service """
    __slots__ = ['_db', '_quart_instance']

    def __init__(self, quart_instance):
        super().__init__()
        self._quart_instance = quart_instance
        self._db : SqliteInterface = None

        self._logger = logging.getLogger(__name__)
        log_format= logging.Formatter(LOGGING_LOG_FORMAT_STRING,
                                      LOGGING_DATETIME_FORMAT_STRING)
        console_stream = logging.StreamHandler()
        console_stream.setFormatter(log_format)
        self._logger.setLevel(LOGGING_DEFAULT_LOG_LEVEL)
        self._logger.addHandler(console_stream)

    def _initialise(self) -> bool:

        build = f"V{RELEASE_VERSION}-{BUILD_VERSION}{BUILD_TAG}"

        self._logger.info('ITEMS Accounts Microservice %s', build)
        self._logger.info(COPYRIGHT_TEXT)
        self._logger.info(LICENSE_TEXT)

        if not self._manage_configuration():
            return False

        self._logger.info('Setting logging level to %s',
                          ThreadafeConfiguration().get_entry("logging",
                                                             "log_level"))
        self._logger.setLevel(ThreadafeConfiguration().get_entry("logging",
                                                                 "log_level"))

        engine : str = ThreadafeConfiguration().get_entry("backend", "engine")
        if engine == 'internalDB' and not self._open_internal_database():
            return False

        basic_auth_blueprint = create_basic_auth_blueprint(self._db)
        self._quart_instance.register_blueprint(basic_auth_blueprint)

        return True

    async def _main_loop(self) -> None:
        ''' Abstract method for main application. '''
        await asyncio.sleep(0.1)

    def _shutdown(self):
        ''' Abstract method for application shutdown. '''

    def _open_internal_database(self) -> bool:
        self._logger.info("Back-end is using internal database...")

        status : bool = False

        filename : str = ThreadafeConfiguration().get_entry(
            'backend', 'internal_db_filename')
        create_if_missing : str = ThreadafeConfiguration().get_entry(
            'backend', 'create_db_if_missing')

        self._db = SqliteInterface(self._logger, filename)

        if os.path.isfile(filename):

            if not self._db.valid_database():
                self._logger.critical("Database file '%s' is not valid!",
                                      filename)
            else:
                return True

        else:
            if create_if_missing == 'YES':
                status, err_str = self._db.build_database()
                if not status:
                    self._logger.critical(err_str)

                else:
                    self._logger.info('Database created successfully')
                    return True

            else:
                self._logger.critical(("Database file '%s' doesn't exist and "
                                       "won't get created!"), filename)

        return status

    def _manage_configuration(self) -> bool:
        """
        Manage the service configuration.
        """

        config_file = os.getenv("ITEMS_ACCOUNTS_SVC_CONFIG_FILE", None)

        config_file_required : bool = os.getenv(
            "ITEMS_ACCOUNTS_SVC_CONFIG_FILE_REQUIRED", None)
        config_file_required = False if not config_file_required \
                               else config_file_required

        if not config_file and config_file_required:
            print("[FATAL ERROR] Configuration file missing!")
            return False

        ThreadafeConfiguration().configure(CONFIGURATION_LAYOUT, config_file,
                                           config_file_required)

        try:
            ThreadafeConfiguration().process_config()

        except ValueError as ex:
            self._logger.critical("Configuration error : %s", ex)
            return False

        self._logger.info("Configuration")
        self._logger.info("=============")
        self._logger.info("[logging]")
        self._logger.info("=> Logging log level : %s",
                          ThreadafeConfiguration().logging_log_level())
        self._logger.info("[Backend]")
        self._logger.info("=> Engine            : %s",
                          ThreadafeConfiguration().backend_engine())

        if ThreadafeConfiguration().backend_engine() == "internalDB":
            self._logger.info("=> Database filename : %s",
                              ThreadafeConfiguration().get_entry(
                                "backend", "internal_db_filename"))

            self._logger.info("=> create if missing : %s",
                              ThreadafeConfiguration().backend_create_db_if_missing())

        return True
