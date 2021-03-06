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
from http import HTTPStatus
import json
import logging
import mimetypes
import requests
from types import SimpleNamespace
from quart import Blueprint, make_response, request, render_template, Response
from config import Config
from items_exception import ItemsException
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING
from web_base_view import WebBaseView

def create_data_view_blueprint(config : Config) -> Blueprint:
    view = View(config)

    blueprint = Blueprint('data_view', __name__)

    @blueprint.route('/testcases', methods=['GET'])
    async def home_request():
        # pylint: disable=unused-variable
        return await view.testcases_handler(request)

    return blueprint

class View(WebBaseView):
    ''' Data view container class. '''

    TEST_CASES_LOGIN_PAGE = "test_cases.html"
    TEMPLATE_INTERNAL_ERROR_PAGE = "internal_server_error.html"

    def __init__(self, config : Config):
        super().__init__(config)

        self._logger = logging.getLogger(__name__)
        log_format= logging.Formatter(LOGGING_LOG_FORMAT_STRING,
                                      LOGGING_DATETIME_FORMAT_STRING)
        console_stream = logging.StreamHandler()
        console_stream.setFormatter(log_format)
        self._logger.setLevel(LOGGING_DEFAULT_LOG_LEVEL)
        self._logger.addHandler(console_stream)

        mimetypes.init()

    async def testcases_handler(self, api_request) -> Response:
        """
        Handler method for viewing test cases.

        parameters:
            api_request - REST API request object

        returns:
            Instance of Quart Response class.
        """

        try:
            if not self._has_auth_cookies() or not self._validate_cookies():
                redirect = self._generate_redirect('login')
                return await make_response(redirect)

        except ItemsException as ex:
                self._logger.error('Internal Error: %s', ex)
                return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

        return await render_template(self.TEST_CASES_LOGIN_PAGE)
