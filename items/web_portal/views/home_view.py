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

def create_home_blueprint(config : Config) -> Blueprint:
    view = View(config)

    blueprint = Blueprint('home', __name__)

    @blueprint.route('/', methods=['GET'])
    async def home_request():
        # pylint: disable=unused-variable
        return await view.home_handler(request)

    @blueprint.route('/login', methods=['GET', 'POST'])
    async def login_request():
        # pylint: disable=unused-variable
        return await view.login_handler(request)

    return blueprint

class View(WebBaseView):
    ''' Home view container class. '''

    TEMPLATE_LOGIN_PAGE = "login.html"
    TEMPLATE_HOME_PAGE = "home.html"
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

    async def home_handler(self, api_request) -> Response:
        """
        Handler method for home page (e.g. '/').

        parameters:
            api_request - REST API request object

        returns:
            Instance of Quart Response class.
        """

        try:
            if not self._has_auth_cookies() or not self._validate_cookies():
                print('redirrrrr')
                redirect = self._generate_redirect('login')
                return await make_response(redirect)

        except ItemsException as ex:
                self._logger.error('Internal Error: %s', ex)
                return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

        return await render_template(self.TEMPLATE_HOME_PAGE)

    async def login_handler(self, api_request) -> Response:
        """
        Handler method for login page.

        parameters:
            api_request - REST API request object

        returns:
            Instance of Quart Response class.
        """

        try:
            if self._has_auth_cookies() and self._validate_cookies():
                redirect = self._generate_redirect('')
                response = await make_response(redirect)
                return response

        except ItemsException as ex:
                self._logger.error('Internal Error: %s', ex)
                return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

        if api_request.method == "GET":
            return await render_template(self.TEMPLATE_LOGIN_PAGE)

        # If not a GET method, it can only be a POST, so handle that!
        user_email = (await api_request.form).get('user_email')
        password = (await api_request.form).get('password')

        if user_email and password:
            auth_body = {
                "email_address": user_email,
                "password": password
            }
            url = f"{self._config.gateway_api.base_url}/handshake/basic_authenticate"

            try:
                response = requests.post(url, json = auth_body)

            except requests.exceptions.ConnectionError as ex:
                raise ItemsException('Connection to gateway api timed out') from ex

            body = json.loads(response.content,
                              object_hook=lambda d: SimpleNamespace(**d))

            if response.status_code == HTTPStatus.NOT_ACCEPTABLE:
                except_str = ("Internal error communicating with gateway: "
                          f"{body.error}")
                self._logger.error(except_str)
                return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

            if body.status == 1:
                redirect = self._generate_redirect('')
                response = await make_response(redirect)
                response.set_cookie(self.COOKIE_USER, user_email)
                response.set_cookie(self.COOKIE_TOKEN, body.token)
                return response

            else:
                error_msg = "Invalid username/password"
                return await render_template(self.TEMPLATE_LOGIN_PAGE,
                                             generate_error_msg = True,
                                             error_msg = error_msg)

        return self._generate_redirect('/login')
