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
from http import HTTPStatus
import json
import logging
import mimetypes
import requests
from types import SimpleNamespace
from quart import Blueprint, make_response, request, render_template, Response
from threadsafe_configuration import ThreadafeConfiguration as Configuration
from items_exception import ItemsException

from web_base_view import WebBaseView

def create_home_blueprint(logger : logging.Logger) -> Blueprint:
    view = View(logger)

    blueprint = Blueprint('home', __name__)

    @blueprint.route('/', methods=['GET'])
    async def home_request():
        # pylint: disable=unused-variable
        return await view.home_handler(request)

    @blueprint.route('/login', methods=['GET', 'POST'])
    async def login_request():
        # pylint: disable=unused-variable
        return await view.login_handler(request)

    @blueprint.route('/project_select', methods=['GET', 'POST'])
    async def project_selection_request():
        # pylint: disable=unused-variable
        return await view.project_selection_handler(request)

    @blueprint.route('/logout', methods=['GET'])
    async def logout_request():
        # pylint: disable=unused-variable
        return await view.logout_handler(request)

    return blueprint

class View(WebBaseView):
    ''' Home view container class. '''

    TEMPLATE_LOGIN_PAGE = "login.html"
    TEMPLATE_HOME_PAGE = "home.html"
    TEMPLATE_PROJECT_SELECTION_PAGE = "project_select.html"
    TEMPLATE_INTERNAL_ERROR_PAGE = "internal_server_error.html"

    def __init__(self, logger : logging.Logger):
        super().__init__()

        self._logger = logger.getChild(__name__)

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
            url = f"{Configuration().internal_api_gateway}/handshake/basic_authenticate"

            try:
                response = requests.post(url, json = auth_body)

            except requests.exceptions.ConnectionError as ex:
                self._logger.error('Connection to gateway api timed out: %s',
                                   str(ex))
                return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

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

    async def logout_handler(self, api_request):

        try:
            if not self._has_auth_cookies() or not self._validate_cookies():
                redirect = self._generate_redirect('login')
                return await make_response(redirect)

        except ItemsException as ex:
                self._logger.error('Internal Error: %s', ex)
                return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

        msg_body = {
            "email_address": request.cookies.get(self.COOKIE_USER),
            "token": request.cookies.get(self.COOKIE_TOKEN)
        }

        url = f"{self._config.gateway_api.base_url}/handshake/logout"

        try:
            response = requests.post(url, json = msg_body)

        except requests.exceptions.ConnectionError:
            return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

        if response.status_code == HTTPStatus.NOT_ACCEPTABLE:
            self._logger.error("Internal error communicating with gateway")
            return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

        redirect = self._generate_redirect('')
        response = await make_response(redirect)
        response.set_cookie(self.COOKIE_USER, '', expires = 0)
        response.set_cookie(self.COOKIE_TOKEN, '', expires = 0)

        return response

    async def project_selection_handler(self, api_request) -> Response:
        """
        Handler method for the project selection page.

        parameters:
            api_request - REST API request object

        returns:
            Instance of Quart Response class.
        """

        response = await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

        if api_request.method == "GET":
            response = await self._handle_project_select_get(api_request)

        elif api_request.method == "POST":
            selected_project = (await api_request.form).get('project')
            print('selected project: ', selected_project)

            response = await self._handle_project_select_post(api_request)

            return response

            return await render_template(self.TEMPLATE_PROJECT_SELECTION_PAGE)

            response = await self._handle_project_select_post(api_request)

        return response

    async def _generate_project_selection_list(self, raw_data):

        first_entry = True
        built_projects = {}

        for entry in raw_data.get('projects'):
            project_name = entry.get('name')
            project_entry = {'name': project_name}

            if first_entry:
                project_entry['selected'] = True
                first_entry = False

            built_projects[project_name] = project_entry

        return built_projects

    async def _handle_project_select_get(self, api_request):

        url = f"{self._config.gateway_api.base_url}/handshake/get_projects"

        try:
            response = requests.get(url)

            if response.status_code == HTTPStatus.OK:
                projects_list = await self._generate_project_selection_list(
                    response.json())

                has_error = False
                error_msg = ""

                if not projects_list:
                    has_error = True
                    error_msg="ERROR: No selectable projects!"

                response = await render_template(
                    self.TEMPLATE_PROJECT_SELECTION_PAGE,
                    projects = projects_list, has_error = has_error,
                    error_msg = error_msg)

            else:
                err_msg = "Internal error, please refresh to try again"
                response = await render_template(
                    self.TEMPLATE_PROJECT_SELECTION_PAGE, projects = {},
                    has_error = True, error_msg = err_msg)

        except requests.exceptions.ConnectionError:
            err_msg = "Internal error, please refresh to try again"
            response = await render_template(
                self.TEMPLATE_PROJECT_SELECTION_PAGE, projects = {},
                has_error = True, error_msg = err_msg)

        return response

    async def _handle_project_select_post(self, api_request):

        selected_project = (await api_request.form).get('project')

        if not selected_project:
            redirect = self._generate_redirect('project_select')
            response = await make_response(redirect)

        else:
            url = (f"{self._config.gateway_api.base_url}"
                    "/handshake/select_project")

            try:
                response = requests.post(url)

                if response.status_code == HTTPStatus.OK:

                    redirect = self._generate_redirect('')
                    response = await make_response(redirect)

                else:
                    err_msg = "Internal error, please refresh to try again"
                    response = await render_template(
                        self.TEMPLATE_PROJECT_SELECTION_PAGE, projects = {},
                        has_error = True, error_msg = err_msg)

            except requests.exceptions.ConnectionError:
                err_msg = "Internal error, please refresh to try again"
                response = await render_template(
                    self.TEMPLATE_PROJECT_SELECTION_PAGE, projects = {},
                    has_error = True, error_msg = err_msg)

        return response
