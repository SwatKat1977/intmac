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
from application import Application
from config import Config
from redis_interface import RedisInterface
from views.handshake_view import create_handshake_blueprint
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING
from version import BUILD_TAG, BUILD_VERSION, RELEASE_VERSION

class GatewayApiApplication(Application):
    """ Gateway API application class """

    title_text = 'ITEMS Gateway-API Microservice %s'
    copyright_text = 'Copyright 2014-2021 Integrated Test Management Suite'
    license_text = 'Licensed under the Apache License, Version 2.0'

    def __init__(self, quart_instance):
        super().__init__()
        self._config = None
        self._quart_instance = quart_instance
        self._redis = None

        self._logger = logging.getLogger(__name__)
        log_format= logging.Formatter(LOGGING_LOG_FORMAT_STRING,
                                      LOGGING_DATETIME_FORMAT_STRING)
        console_stream = logging.StreamHandler()
        console_stream.setFormatter(log_format)
        self._logger.setLevel(LOGGING_DEFAULT_LOG_LEVEL)
        self._logger.addHandler(console_stream)

    def _initialise(self) -> bool:

        build = f"V{RELEASE_VERSION}-{BUILD_VERSION}{BUILD_TAG}"

        self._logger.info(self.title_text, build)
        self._logger.info(self.copyright_text)
        self._logger.info(self.license_text)

        self._config = Config().read_config("./config.ini")

        self._logger.info('Opening REDIS database...')

        if not self._open_database():
            self._logger.critical('-> REDIS database database open: FAILED')
            return False

        self._logger.info('-> REDIS database database open: SUCCESSFUL')

        auth_view_blueprint = create_handshake_blueprint()
        self._quart_instance.register_blueprint(auth_view_blueprint)

        return True

    def _main_loop(self) -> None:
        """ Abstract method for main application. """

    def _shutdown(self):
        """ Abstract method for application shutdown. """

    def _open_database(self) -> bool:
        """
        Open a connection to the REDIS database.

        returns:
            bool - Status of database open.
        """

        status = False

        self._redis = RedisInterface(self._config.database.database_host_name,
                                     self._config.database.database_host_port)

        if self._redis.initialise():
            status = True

        return status
