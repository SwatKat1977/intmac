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
from quart import Blueprint, make_response, request, render_template, Response
from threadsafe_configuration import ThreadafeConfiguration as Configuration
from items_exception import ItemsException
from threadsafe_configuration import ThreadafeConfiguration
from web_base_view import WebBaseView

def create_data_view_blueprint(logger : logging.Logger) -> Blueprint:
    view = View(logger)

    blueprint = Blueprint('data_view', __name__)

    @blueprint.route('/testcases', methods=['GET'])
    async def testcases_request():
        return await view.testcases_handler(request)

    return blueprint

class View(WebBaseView):
    ''' Data view container class. '''

    TEST_CASES_LOGIN_PAGE = "test_cases.html"
    TEMPLATE_INTERNAL_ERROR_PAGE = "internal_server_error.html"

    def __init__(self, logger : logging.Logger):
        super().__init__()

        self._logger = logger.getChild(__name__)

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

        base_path : str = ThreadafeConfiguration().internal_api_gateway

        request : dict = {
            "project_id": 1
        }
        url : str = (f'{base_path}/internal/testcases/get_testsuites')

        api_response = await self._call_api_get(url, request)

        if api_response.status_code != HTTPStatus.OK:
            self._logger.critical("Invalid Gateway svc request '%s' - Reason: %s",
                                  url, api_response.body['error'])
            response_json = {
                "status": 0,
                'error': api_response.body['error']
            }

            return Response(json.dumps(response_json),
                            status=HTTPStatus.INTERNAL_SERVER_ERROR,
                            mimetype=mimetypes.types_map['.json'])

        testsuites = self._organise_testsuites(api_response.body['data'], depth=0)

        request : dict = {
            "project_id": 1
        }
        url : str = (f'{base_path}/internal/testcases/get_testcases_overview')

        api_response = await self._call_api_get(url, request)

        if api_response.status_code != HTTPStatus.OK:
            self._logger.critical("Invalid Gateway svc request '%s' - Reason: %s",
                                  url, api_response.body['error'])
            response_json = {
                "status": 0,
                'error': api_response.body['error']
            }

            return Response(json.dumps(response_json),
                            status=HTTPStatus.INTERNAL_SERVER_ERROR,
                            mimetype=mimetypes.types_map['.json'])

        has_testcases : bool = False
        if testsuites or api_response.body:
            start_depth : int = 0
            start_parent : int = -1

            html_code : str = self._generate_test_overview_for_depth(
                testsuites, api_response.body, start_depth, start_parent)
            print(html_code)
            has_testcases = True

        '''
        {% if has_testcases == True %}
        {% testcases %}

        err_msg = "Internal error, please refresh to try again"
        response = await render_template(
                    self.TEMPLATE_PROJECT_SELECTION_PAGE, projects = {},
                    has_error = True, error_msg = err_msg)
        '''

        return await render_template(self.TEST_CASES_LOGIN_PAGE,
                                     has_testcases=has_testcases,
                                     testcases=html_code)

    def _organise_testsuites(self, test_suites, depth : int,
                                  parent : int = -1):
        """
        Recursive method to organise the testsuites and calculate/add the depth
        dictionary key and value.

        parameters:
            test_suites : List of test suites dictonaries\n
            depth : Depth of the test suite (starting from 0)\n
            parent : (Optional) Parent suite to organise from

        returns:
            List of dictionaries representing test suites with the extra depth
            field.
        """
        suites : list = []

        root_entries = [suite for suite in test_suites if suite['parent'] == parent]
        if not root_entries:
            return suites

        for entry in root_entries:
            entry['depth'] = depth
            suites.append(entry)
            new_entries = self._organise_testsuites(test_suites,
                                                    depth=depth+1,
                                                    parent=entry['id'])
            if new_entries:
                suites = [*suites, *new_entries]

        return suites

    def _generate_test_overview_for_depth(self, test_suites : list,
                                          test_cases : list, depth : int, 
                                          parent : int) -> str:

        entries = [suite for suite in test_suites if suite['depth'] == depth \
                   and suite['parent'] == parent]
        html_code : str = ""
        outside_indent : str = ' ' * ((depth +2) * 4)
        inside_indent : str = ' ' * ((depth +3) * 4)

        '''
            <div class="tree well">
        <ul>
            <li>
                <span><i class="fas fa-folder-open"></i></span> Top-Level Parent
                <ul>
                    <li>
                        <span><i class="fas fa-folder-open"></i></span> Child #1
                        <ul>
                        </ul>
                    </li>
        '''

        if entries:
            for entry in entries:
                html_code += f"{outside_indent}<li>\n" + \
                             f"{inside_indent}<span><i class='fas fa-folder-open'>" + \
                             f"</i></span> {entry['name']}\n" + \
                             f"{inside_indent}<ul>\n"
                html_code += self._generate_test_overview_for_depth(
                    test_suites,test_cases, depth + 1, entry['id'])
                html_code += f"{inside_indent}</ul>\n" + \
                             f"{outside_indent}</li>\n"
            '''
                    <li>
                        <span><i class="fas fa-folder-open"></i></span> Child #1
                        <ul>
                            <li>
                                <span><i class="fas fa-book"></i></span><a href="">Test Case #1</a>
                            </li>
                        </ul>
                    </li>
            '''

        return html_code

    def _generate_testcase_overview_tree(self, test_suites : list,
                                         test_cases : list):
        depth : int = 0
        parent : int = -1

        return self._generate_test_overview_for_depth(test_suites, test_cases,
                                                      depth, parent)

        entries = [suite for suite in test_suites if suite['depth'] == depth]
        print("Entries", entries)

        '''
<body>
    <div class="tree well">
        <ul>
            <li>
                <span><i class="fas fa-folder-open"></i></span> Top-Level Parent
                <ul>
                    <li>
                        <span><i class="fas fa-folder-open"></i></span> Child #1
                        <ul>
                            <li>
                                <span><i class="fas fa-book"></i></span><a href="">Test Case #1</a>
                            </li>
                        </ul>
                    </li>
                    <li>
                        <span><i class="fas fa-folder-open"></i></span> Child #2
                        <ul>
                            <li>
                                <span><i class="fas fa-book"></i></span><a href="">Test Case #2</a>
                            </li>
                            <li>
                                <span><i class="fas fa-folder-open"></i></span> Child #2.1
                                <ul>
                                    <li>
                                        <span><i class="fas fa-folder-open"></i></span>Child #2.1.1
                                        <ul>
                                            <li>
                                                <span><i class="fas fa-book"></i></span><a href="">Test Case #3</a>
                                            </li>
                                            <li>
                                                <span><i class="fas fa-book"></i></span><a href="">Test Case #4</a>
                                            </li>
                                         </ul>
                                    </li>
                                    <li>
                                        <span><i class="fas fa-book"></i></span><a href="">Test Case #5</a>
                                    </li>
                                    <li>
                                        <span><i class="fas fa-book"></i></span><a href="">Test Case #6</a>
                                    </li>
                                </ul>
                            </li>
                            <li>
                                <span><i class="fas fa-book"></i></span><a href="">Test Case #7</a>
                            </li>
                        </ul>
                    </li>
                </ul>
            </li>
        </ul>
    </div>
</body>
        '''
