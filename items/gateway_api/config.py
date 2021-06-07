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
from configparser import ConfigParser
from dataclasses import dataclass
import logging
import os
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING

@dataclass
class DatabaseConfigData:
    """ Database configuration data """

    database_host_name : str = 'localhost'
    database_host_port : int = 6379

@dataclass
class ConfigData:
    """ Main configuration data"""

    database : DatabaseConfigData

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
        db_data = DatabaseConfigData('localhost', 6379)

        # Check for Override Environment variables
        if (db_host_name := os.getenv('ITEMS_DATABASE_HOST_NAME')) is not None:
            db_data.database_host_name = db_host_name

        if (db_host_port := os.getenv('ITEMS_DATABASE_HOST_PORT')) is not None:
            db_data.database_host_port = int(db_host_port)

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

        self._logger.info("+=== Configuration Settings ===+")
        self._logger.info("+==============================+")
        self._logger.info("Database Settings :->")
        self._logger.info("+= Database host name : %s",
                          db_data.database_host_name)
        self._logger.info("+= Database host port : %s",
                          db_data.database_host_port)
        self._logger.info("+==============================+")

        return ConfigData(db_data)
