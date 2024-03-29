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
import logging
import os
from base_application import BaseApplication, COPYRIGHT_TEXT, LICENSE_TEXT
from configuration_layout import CONFIGURATION_LAYOUT
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING
from threadsafe_configuration import ThreadafeConfiguration as Configuration
from version import BUILD_TAG, BUILD_VERSION, RELEASE_VERSION
from views.home_view import create_home_blueprint
from views.data_view import create_data_view_blueprint

class Application(BaseApplication):
    ''' Web portal application class '''

    def __init__(self, quart_instance):
        super().__init__()
        self._quart_instance = quart_instance

        self._logger = logging.getLogger(__name__)
        log_format= logging.Formatter(LOGGING_LOG_FORMAT_STRING,
                                      LOGGING_DATETIME_FORMAT_STRING)
        console_stream = logging.StreamHandler()
        console_stream.setFormatter(log_format)
        self._logger.setLevel(LOGGING_DEFAULT_LOG_LEVEL)
        self._logger.addHandler(console_stream)

    def _initialise(self) -> bool:

        build = f"V{RELEASE_VERSION}-{BUILD_VERSION}{BUILD_TAG}"

        self._logger.info('ITEMS Web Portal Service %s', build)
        self._logger.info(COPYRIGHT_TEXT)
        self._logger.info(LICENSE_TEXT)

        if not self._manage_configuration():
            return False

        auth_view_blueprint = create_home_blueprint(self._logger)
        self._quart_instance.register_blueprint(auth_view_blueprint)

        data_view_blueprint = create_data_view_blueprint(self._logger)
        self._quart_instance.register_blueprint(data_view_blueprint)

        return True

    async def _main_loop(self) -> None:
        ''' Abstract method for main application. '''

    def _shutdown(self):
        ''' Abstract method for application shutdown. '''

    def _manage_configuration(self) -> bool:
        """
        Manage the service configuration.
        """

        config_file = os.getenv("ITEMS_WEB_PORTAL_SVC_CONFIG_FILE", None)

        config_file_required : bool = os.getenv(
            "ITEMS_WEB_PORTAL_SVC_CONFIG_FILE_REQUIRED", None)
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
                          Configuration().logging_log_level)
        self._logger.info("[Internal APIs]")
        self._logger.info("=> Gateway Service : %s",
                          Configuration().internal_api_gateway)

        return True
