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
import time
from base_application import BaseApplication, COPYRIGHT_TEXT, LICENSE_TEXT
from config import Configuration
from configuration_layout import CONFIGURATION_LAYOUT
from redis_interface import RedisInterface
from views.handshake_view import create_handshake_blueprint
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING
from version import BUILD_TAG, BUILD_VERSION, RELEASE_VERSION

class GatewayApiApplication(BaseApplication):
    """ Gateway API application class """

    def __init__(self, quart_instance):
        super().__init__()
        self._quart_instance = quart_instance
        self._sessions : RedisInterface = None

        self._logger = logging.getLogger(__name__)
        log_format= logging.Formatter(LOGGING_LOG_FORMAT_STRING,
                                      LOGGING_DATETIME_FORMAT_STRING)
        console_stream = logging.StreamHandler()
        console_stream.setFormatter(log_format)
        self._logger.setLevel(LOGGING_DEFAULT_LOG_LEVEL)
        self._logger.addHandler(console_stream)

    def _initialise(self) -> bool:

        build = f"V{RELEASE_VERSION}-{BUILD_VERSION}{BUILD_TAG}"

        self._logger.info('ITEMS Gateway Service %s', build)
        self._logger.info(COPYRIGHT_TEXT)
        self._logger.info(LICENSE_TEXT)

        if not self._manage_configuration():
            return False

        self._logger.info('Setting logging level to %s',
                          Configuration().get_entry("logging", "log_level"))
        self._logger.setLevel(Configuration().get_entry("logging",
                                                        "log_level"))

        self._logger.info('Opening REDIS database...')

        if not self._connect_to_sessions_redis():
            return False

        auth_view_blueprint = create_handshake_blueprint(self._sessions)
        self._quart_instance.register_blueprint(auth_view_blueprint)

        return True

    async def _main_loop(self) -> None:
        ''' Method for main application. '''
        await asyncio.sleep(0.1)

    def _connect_to_sessions_redis(self) -> bool:
        """
        Open a connection to the REDIS database.

        returns:
            bool - Status of database open.
        """

        self._sessions = RedisInterface(
            self._logger,
            Configuration().get_entry("sessions_redis", "host"),
            Configuration().get_entry("sessions_redis", "port"),
            Configuration().get_entry("sessions_redis", "password"))

        redis_connected : bool = False
        redis_connect_tries : int = Configuration().get_entry(
            "sessions_redis", "retries")
        redis_connect_wait : int = 10

        while not redis_connected and redis_connect_tries != 0:
            redis_connected = self._sessions.initialise()

            if not redis_connected:
                self._logger.error(
                    "Failed to connect to redis, retrying in %s seconds",
                    redis_connect_wait)
                time.sleep(redis_connect_wait)
                redis_connect_wait = redis_connect_wait * 2
                redis_connect_tries -= 1

        if not redis_connected:
            self._logger.critical("Failed to connect to redis, aborting...")
            return False

        self._logger.info("Connected to sessions redis database")
        return True

    def _manage_configuration(self) -> bool:
        """
        Manage the service configuration.
        """

        config_file = os.getenv("ITEMS_GATEWAY_SVC_CONFIG_FILE", None)

        config_file_required : bool = os.getenv(
            "ITEMS_GATEWAY_SVC_CONFIG_FILE_REQUIRED", None)
        config_file_required = False if not config_file_required \
                               else config_file_required

        if not config_file and config_file_required:
            print("[FATAL ERROR] Configuration file missing!")
            return False

        Configuration().configure(CONFIGURATION_LAYOUT, config_file,
                                  config_file_required)

        try:
            Configuration().process_config()

        except ValueError as ex:
            self._logger.critical("Configuration error : %s", ex)
            return False

        self._logger.info("Configuration")
        self._logger.info("=============")
        self._logger.info("[logging]")
        self._logger.info("=> Logging log level : %s",
                          Configuration().get_entry("logging", "log_level"))
        self._logger.info("[REDIS]")
        self._logger.info("=> Host    : %s",
                          Configuration().get_entry("sessions_redis",
                                                    "host"))
        self._logger.info("=> Port    : %s",
                          Configuration().get_entry("sessions_redis",
                                                    "port"))
        self._logger.info("=> Retries : %s",
                          Configuration().get_entry("sessions_redis",
                                                    "retries"))
        self._logger.info("[Internal APIs]")
        self._logger.info("=> accounts svc : %s",
                          Configuration().get_entry("internal_apis",
                                                    "accounts_svc"))
        self._logger.info("=> CMS svc      : %s",
                          Configuration().get_entry("internal_apis",
                                                    "cms_svc"))

        return True
