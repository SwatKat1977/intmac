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
from base64 import b64encode
from http import HTTPStatus
import json
import logging
import mimetypes
import uuid
from quart import Blueprint, request, Response
import requests
from base_view import BaseView
from config import ConfigData
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING
from logon_type import LogonType
from redis_interface import RedisInterface

tests = [
    {
        'id': 0,
        'name': b'root',
        'parent': -1,
        'type': 'filter'
    },
    {
        'id': 1,
        'name': b'UI Tests',
        'parent': 0,
        'type': 'filter'
    },
    {
        'id': 2,
        'name': b'API Tests',
        'parent': 0,
        'type': 'filter'
    },
    {
        'id': 3,
        'name': b'Items Login Successful',
        'parent': 2,
        'type': 'test case'
    },
    {
        'id': 4,
        'name': b'Items Login Unsuccessful',
        'parent': 2,
        'type': 'test case'
    }
]

test_sets = [
    {
        'id': 0,
        'name': b'root',
        'parent': -1,
        'type': 'filter'
    },
    {
        'id': 0,
        'name': b'RELEASE_1.0.0',
        'parent': 0,
        'type': 'filter'
    },
    {
        'id': 0,
        'name': b'RELEASE_1.0.1',
        'parent': 0,
        'type': 'filter'
    },
]

def create_handshake_blueprint(config : ConfigData, sessions : RedisInterface):
    view = View(config, sessions)

    blueprint = Blueprint('handshake_api', __name__)

    @blueprint.route('/handshake/authenticate', methods=['POST'])
    async def authenticate_user_request():
        # pylint: disable=unused-variable
        return await view.authenticate_user_handler(request)

    @blueprint.route('/handshake/logout', methods=['POST'])
    async def logout_user_request():
        # pylint: disable=unused-variable
        return await view.logout_user_handler(request)

    @blueprint.route('/handshake/valid_token', methods=['GET'])
    async def valid_token_request():
        # pylint: disable=unused-variable
        return await view.valid_token_handler(request)

    @blueprint.route('/handshake/get_projects', methods=['GET'])
    async def get_projects_request():
        # pylint: disable=unused-variable
        return await view.get_projects_handler(request)

    @blueprint.route('/handshake/select_project', methods=['POST'])
    async def select_project_request():
        # pylint: disable=unused-variable
        return await view.select_projects_handler(request)

    return blueprint

class View(BaseView):
    ''' Handshake view container class. '''

    basicAuthenticateRequestSchema = \
    {
        "$schema": "http://json-schema.org/draft-07/schema#",

        "type" : "object",
        "additionalProperties" : False,

        "properties":
        {
            "email_address":
            {
                "type" : "string"
            },
            "password":
            {
                "type" : "string"
            },
        },
        "required" : ["email_address", "password"]
    }

    tokenValidationCheckRequestSchema = \
    {
        "$schema": "http://json-schema.org/draft-07/schema#",

        "type" : "object",
        "additionalProperties" : False,

        "properties":
        {
            "email_address":
            {
                "type" : "string"
            },
            "token":
            {
                "type" : "string"
            },
        },
        "required" : ["email_address", "token"]
    }

    def __init__(self, config : ConfigData, sessions : RedisInterface):
        self._config = config
        self._sessions = sessions

        self._logger = logging.getLogger(__name__)
        log_format= logging.Formatter(LOGGING_LOG_FORMAT_STRING,
                                      LOGGING_DATETIME_FORMAT_STRING)
        console_stream = logging.StreamHandler()
        console_stream.setFormatter(log_format)
        self._logger.setLevel(LOGGING_DEFAULT_LOG_LEVEL)
        self._logger.addHandler(console_stream)

        mimetypes.init()

    async def authenticate_user_handler(self, api_request):

        request_obj, err_msg = await self._convert_json_body_to_object(
            api_request, self.basicAuthenticateRequestSchema)

        if not request_obj:

            response_json = {
                'status': 'BAD',
                'error': err_msg
            }
            response_status = HTTPStatus.NOT_ACCEPTABLE

        else:
            auth_request = {
                "email_address": request_obj.email_address,
                "password": request_obj.password
            }
            auth_url = (f"{self._config.auth_service.base_url}"
                         "/basic_auth/authenticate")

            response = requests.post(auth_url, json=auth_request)

            if response.status_code != HTTPStatus.OK:
                self._logger.error("Auth request invalid:\n  %s\n  Reason:%s",
                                   auth_request, response.text)
                response_status = HTTPStatus.INTERNAL_SERVER_ERROR
                response_json = {
                    "status": "OK",
                    "token": "c7dd0a54-baea-11eb-8529-0242ac130003"
                }

            else:
                response_status = HTTPStatus.OK

                body = response.json()
                if not body.get("status"):
                    response_json = {
                        "status":  0,
                        "error": body.get("error")
                    }

                else:
                    token = uuid.uuid4().hex
                    self._sessions.add_auth_session(
                        request_obj.email_address, token, LogonType.BASIC)

                    response_json = {
                        "status": 1,
                        "token": token
                    }
                    self._logger.info("User '%s' logged in",
                                      request_obj.email_address)

        return Response(json.dumps(response_json), status = response_status,
                                   mimetype = mimetypes.types_map['.json'])

    async def logout_user_handler(self, api_request):
        """ PLACEHOLDER """
        # pylint: disable=unused-argument

        self._logger.debug("Logout is not implemented yet")
        return Response('', status=HTTPStatus.OK,
                        mimetype=mimetypes.types_map['.txt'])


    async def valid_token_handler(self, api_request):
        """
        Endpoint to check to see if a token is valid.

        parameters:
            api_request - Request from Quart

        returns:
            Response instance
        """

        request_obj, err_msg = await self._convert_json_body_to_object(
            api_request, self.tokenValidationCheckRequestSchema)

        if not request_obj:

            response_json = { 'status': 'BAD REQUEST' }
            response_status = HTTPStatus.NOT_ACCEPTABLE

        else:
            valid = self._sessions.is_valid_session(request_obj.email_address,
                                                    request_obj.token)
            response_json = { "status": "VALID" if valid else "INVALID" }
            response_status = HTTPStatus.NOT_ACCEPTABLE

        return Response(json.dumps(response_json), response_status,
                        mimetype=mimetypes.types_map['.json'])

    async def get_projects_handler(self, api_request):
        """ PLACEHOLDER """
        # pylint: disable=unused-argument

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

    async def select_projects_handler(self, api_request):
        """ PLACEHOLDER """
        # pylint: disable=unused-argument

        self._logger.debug("selectprojects returns a hard-coded response")

        def generate():
            yield b'{'

            try:
                tests_iter = tests.__iter__()

                previous = next(tests_iter)

                yield b'"test_cases": ['

                for entry in tests_iter:
                    name = previous.get('name')

                    yield (
                        "{"
                        f"\"id\":{previous.get('id')}, "
                        "\"name\":"
                        "\"" + b64encode(name).decode('ascii') + "\","
                        f"\"parent\":{previous.get('parent')}, "
                        f"\"type\":\"{previous.get('type')}\""
                        "},"
                    ).encode("UTF-8")
                    previous = entry

                yield (
                    "{"
                    f"\"id\":{previous.get('id')}, "
                    "\"name\":"
                    "\"" + b64encode(name).decode('ascii') + "\","
                    f"\"parent\":{previous.get('parent')}, "
                    f"\"type\":\"{previous.get('type')}\""
                    "}],"
                ).encode("UTF-8")

            except StopIteration:
                yield b'"test_cases": [],'

            try:
                test_sets_iter = test_sets.__iter__()

                previous = next(test_sets_iter)

                yield b'"test_sets": ['

                for entry in test_sets_iter:
                    name = previous.get('name')

                    yield (
                        "{"
                        f"\"id\":{previous.get('id')}, "
                        "\"name\":"
                        "\"" + b64encode(name).decode('ascii') + "\","
                        f"\"parent\":{previous.get('parent')}, "
                        f"\"type\":\"{previous.get('type')}\""
                        "},"
                    ).encode("UTF-8")
                    previous = entry

                yield (
                    "{"
                    f"\"id\":{previous.get('id')}, "
                    "\"name\":"
                    "\"" + b64encode(name).decode('ascii') + "\","
                    f"\"parent\":{previous.get('parent')}, "
                    f"\"type\":\"{previous.get('type')}\""
                    "}]"
                ).encode("UTF-8")

            except StopIteration:
                yield b'"test_sets": []'

            yield b'}'

        return Response(generate(), status=HTTPStatus.OK,
                        content_type='application/json')
