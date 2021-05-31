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
from items_exception import ItemsException
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING

@dataclass
class DatabaseConfigData:
    database_file : str
    fail_on_no_database : bool

@dataclass
class ConfigData:
    database : DatabaseConfigData

class Config:
    __slots__ = ['_logger']

    def __init__(self) -> None:
        self._logger = logging.getLogger(__name__)
        log_format= logging.Formatter(LOGGING_LOG_FORMAT_STRING,
                                      LOGGING_DATETIME_FORMAT_STRING)
        console_stream = logging.StreamHandler()
        console_stream.setFormatter(log_format)
        self._logger.setLevel(LOGGING_DEFAULT_LOG_LEVEL)
        self._logger.addHandler(console_stream)

    def read_config(self, config_file : str):

        config = ConfigParser()

        db_data = DatabaseConfigData('./auth.db', False)

        if config_file and not os.path.isfile(config_file):
            self._logger.warning(("Specified config file '%s' cannot be read, "
                                 "the defaults will be used!"), config_file)

        else:
            config.read(config_file)

            if config.has_section('database'):
                db_section = config['database']

                db_file = db_section.get('database_file')
                if db_file:
                    db_data.database_file = db_file

                fail_on_no_db = db_section.get('fail_no_database', None)
                if fail_on_no_db is not None:
                    db_data.fail_on_no_database = fail_on_no_db

        self._logger.info("+=== Configuration Settings ===+")
        self._logger.info("+==============================+")
        self._logger.info("Database Settings :->")
        self._logger.info("+= database file : %s", db_data.database_file)
        self._logger.info("+= Fail on no db : %s", db_data.fail_on_no_database)
        self._logger.info("+==============================+")

        return ConfigData(db_data)
