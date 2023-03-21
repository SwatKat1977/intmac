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
from configparser import ConfigParser
from dataclasses import dataclass
import logging
import os
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING

DEFAULT_DB_HOST_NAME = 'localhost'
DEFAULT_DB_HOST_PORT = 6379

DEFAULT_AUTH_BASE_URL = 'http://localhost:3030'

@dataclass
class DatabaseConfigData:
    """ Database configuration data """

    database_host_name : str
    database_host_port : int

@dataclass
class AuthServiceConfigData:
    """ Auyh Service configuration data"""

    base_url : str

@dataclass
class ConfigData:
    """ Main configuration data"""

    database : DatabaseConfigData
    auth_service : AuthServiceConfigData

class Config:
    """ Load application configuration """
    __slots__ = ['_logger']

    def __init__(self) -> None:
        self._logger = logging.getLogger(__name__)
        log_format= logging.Formatter(LOGGING_LOG_FORMAT_STRING,
                                      LOGGING_DATETIME_FORMAT_STRING)
        console_stream = logging.StreamHandler()
        console_stream.setFormatter(log_format)
        self._logger.setLevel(LOGGING_DEFAULT_LOG_LEVEL)
        self._logger.addHandler(console_stream)

    def read_config(self, config_file : str) -> ConfigData:
        """
        Read the configuation file for the application.

        Order of precedence:
        1) Override Environment variables
        2) Configuration file
        3) Default values

        parameters:
            config_file - Configuration filename with full path
        """

        config = ConfigParser()

        # Set parameters to default values initially
        db_data = DatabaseConfigData(DEFAULT_DB_HOST_NAME,
                                     DEFAULT_DB_HOST_PORT)
        auth_service_data = AuthServiceConfigData(DEFAULT_AUTH_BASE_URL)

        # Check for Override Environment variables
        if (db_host_name := os.getenv('ITEMS_DATABASE_HOST_NAME')) is not None:
            db_data.database_host_name = db_host_name

        if (db_host_port := os.getenv('ITEMS_DATABASE_HOST_PORT')) is not None:
            db_data.database_host_port = int(db_host_port)

        if (auth_url := os.getenv('ITEMS_AUTH_BASE_URL')) is not None:
            auth_service_data.base_url = auth_url

        # Check for config file and if present read it and use those values
        if config_file and not os.path.isfile(config_file):
            self._logger.warning(("Specified config file '%s' cannot be read, "
                                 "the defaults will be used!"), config_file)

        else:
            config.read(config_file)

            if config.has_section('database'):
                db_section = config['database']

                db_host_name = db_section.get('database_host_name')
                if db_host_name:
                    db_data.database_host_name = db_host_name

                db_host_port = db_section.get('database_host_port')
                if db_host_port:
                    db_data.database_host_port = db_host_port

            if config.has_section('auth_service'):
                auth_service_section = config['auth_service']

                base_url = auth_service_section.get('base_url')
                if base_url:
                    auth_service_data.base_url = base_url

        self._logger.info("+=== Configuration Settings ===+")
        self._logger.info("+==============================+")
        self._logger.info("Database Settings :->")
        self._logger.info("+= Database host name : %s",
                          db_data.database_host_name)
        self._logger.info("+= Database host port : %s",
                          db_data.database_host_port)
        self._logger.info("+==============================+")
        self._logger.info("Authentication Service Settings :->")
        self._logger.info("+= Base URL : %s", auth_service_data.base_url)
        self._logger.info("+==============================+")

        return ConfigData(db_data, auth_service_data)
