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
from quart import Blueprint, request, Response
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING

def create_handshake_blueprint():
    view = View()

    blueprint = Blueprint('handshake_api', __name__)

    @blueprint.route('/handshake/authenticate', methods=['POST'])
    def authenticate_user_request():
        return view.authenticate_user_handler(request)

    @blueprint.route('/handshake/logout', methods=['POST'])
    def logout_user_request():
        return view.logout_user_handler(request)

    @blueprint.route('/handshake/get_projects', methods=['GET'])
    def get_projects_request():
        return view.get_projects_handler(request)

    @blueprint.route('/handshake/select_project', methods=['POST'])
    def select_project_request():
        return view.select_projects_handler(request)

    return blueprint

class View:
    ''' Handshake view container class. '''

    def __init__(self):

        self._logger = logging.getLogger(__name__)
        log_format= logging.Formatter(LOGGING_LOG_FORMAT_STRING,
                                      LOGGING_DATETIME_FORMAT_STRING)
        console_stream = logging.StreamHandler()
        console_stream.setFormatter(log_format)
        self._logger.setLevel(LOGGING_DEFAULT_LOG_LEVEL)
        self._logger.addHandler(console_stream)

        mimetypes.init()

    def authenticate_user_handler(self, api_request):
        self._logger.debug("Generating fake auth response for now...")
        resp = {
            "status": "OK",
            "token": "c7dd0a54-baea-11eb-8529-0242ac130003"

        }
        return Response(json.dumps(resp), status=HTTPStatus.OK,
                        mimetype=mimetypes.types_map['.json'])

    def logout_user_handler(self, api_request):
        self._logger.debug("Logout is not implemented yet")
        return Response('', status=HTTPStatus.OK,
                        mimetype=mimetypes.types_map['.txt'])

    def get_projects_handler(self, api_request):
        self._logger.debug("getprojects currently returns a hard-coded list")

        response = {
            "projects": [
                {
                    "name": "test project #1",
                    "description": "This is test project #1"
                },
                {
                    "name": "test project #2",
                    "description": "This is test project #2"
                }
            ]
        }

        return Response(json.dumps(response), status=HTTPStatus.OK,
                        mimetype=mimetypes.types_map['.json'])

    def select_projects_handler(self, api_request):
        self._logger.debug("selectprojects returns a hard-coded response")

        response = {
            "status": "OK",
            "errors": []
        }
        return Response(json.dumps(response), status=HTTPStatus.OK,
                        mimetype=mimetypes.types_map['.json'])
