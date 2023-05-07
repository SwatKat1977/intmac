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
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING
from version import BUILD_TAG, BUILD_VERSION, RELEASE_VERSION
from views.home_view import create_home_blueprint
from views.data_view import create_data_view_blueprint

class WebPortalApplication(Application):
    ''' Web portal application class '''

    title_text = 'ITEMS Web Portal %s'
    copyright_text = 'Copyright 2014-2021 Integrated Test Management Suite'
    license_text = 'Licensed under the Apache License, Version 2.0'

    def __init__(self, quart_instance):
        super().__init__()
        self._quart_instance = quart_instance
        self._config = None

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

        auth_view_blueprint = create_home_blueprint(self._config)
        self._quart_instance.register_blueprint(auth_view_blueprint)

        data_view_blueprint = create_data_view_blueprint(self._config)
        self._quart_instance.register_blueprint(data_view_blueprint)

        return True

    def _main_loop(self) -> None:
        ''' Abstract method for main application. '''

    def _shutdown(self):
        ''' Abstract method for application shutdown. '''
