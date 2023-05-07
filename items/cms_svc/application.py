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
from base_application import BaseApplication, COPYRIGHT_TEXT, LICENSE_TEXT
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING
from version import BUILD_TAG, BUILD_VERSION, RELEASE_VERSION
from views.placeholder_view import create_placeholder_blueprint

class Application(BaseApplication):
    ''' Contents Management System application class '''

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

        placeholder_blueprint = create_placeholder_blueprint()
        self._quart_instance.register_blueprint(placeholder_blueprint)

        build = f"V{RELEASE_VERSION}-{BUILD_VERSION}{BUILD_TAG}"

        self._logger.info('ITEMS Contents Management Microservice %s', build)
        self._logger.info(COPYRIGHT_TEXT)
        self._logger.info(LICENSE_TEXT)

        return True

    async def _main_loop(self) -> None:
        ''' Abstract method for main application. '''

    def _shutdown(self):
        ''' Abstract method for application shutdown. '''
