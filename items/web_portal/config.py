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

DEFAULT_GATEWAY_BASE_URL = 'http://localhost:2333'

@dataclass
class GatewayConfigData:
    """ Gateway API configuration data"""

    base_url : str

@dataclass
class ConfigData:
    """ Main configuration data"""

    gateway_api : GatewayConfigData

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

        # Set parameters to default values initially
        gateway_data = GatewayConfigData(DEFAULT_GATEWAY_BASE_URL)

        self._check_env_variables(gateway_data)

        # Check for config file and if present read it and use those values
        if config_file and not os.path.isfile(config_file):
            self._logger.warning(("Specified config file '%s' cannot be read, "
                                 "the defaults will be used!"), config_file)

        else:
            self._read_config_file(config_file, gateway_data)

        config = ConfigData(gateway_data)

        self._display_settings(config)

        return config

    def _display_settings(self, config : ConfigData) -> None:

        self._logger.info("+=== Configuration Settings ===+")
        self._logger.info("+==============================+")
        self._logger.info("Gateway API Settings :->")
        self._logger.info("+= Base URL : %s", config.gateway_api.base_url)
        self._logger.info("+==============================+")

    def _check_env_variables(self, settings : GatewayConfigData) -> None:
        # pylint: disable=no-self-use

        # Check for Override Environment variables
        if (base_url := os.getenv('ITEMS_API_BASE_URL')) is not None:
            settings.base_url = base_url

    def _read_config_file(self, config_file : str,
                          gateway_data : GatewayConfigData) -> None:
        # pylint: disable=no-self-use

        config = ConfigParser()

        config.read(config_file)

        if config.has_section('gateway_api'):
            gateway_section = config['gateway_api']

            base_url = gateway_section.get('base_url')
            if base_url:
                gateway_data.base_url = base_url
