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
from quart import Blueprint, request, Response
from base_view import BaseView
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING
from logon_type import LogonType
from sqlite_interface import SqliteInterface

def create_basic_auth_blueprint(sql_interface : SqliteInterface,
                                logger : logging.Logger):
    view = View(sql_interface, logger)

    blueprint = Blueprint('basic_auth_api', __name__)

    @blueprint.route('/basic_auth/authenticate', methods=['POST'])
    async def authenticate_request():
        return await view.authenticate(request)

    return blueprint

class AuthenticateRequest:
    ''' Definition of the basic_auth/authenticate request '''
    #pylint: disable=too-few-public-methods

    identifier = 'identifier'

    schema = \
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

class View(BaseView):
    __slots__ = ['_logger', '_sql_interface']

    def __init__(self, sql_interface : SqliteInterface,
                 logger : logging.Logger) -> None:
        self._sql_interface = sql_interface
        self._logger = logger.getChild(__name__)

    async def authenticate(self, api_request):

        request_obj, err_msg = await self._convert_json_body_to_object(
            api_request, AuthenticateRequest.schema)

        if not request_obj:

            response_json = {
                'status': 0,
                'error': err_msg
            }
            response_status = HTTPStatus.NOT_ACCEPTABLE

        else:
            try:
                user_id, _ = self._sql_interface.valid_user_to_logon(
                    request_obj.email_address, LogonType.BASIC)

                if user_id:
                    status, err_str = self._sql_interface.basic_user_authenticate(
                        user_id, request_obj.password)

                    response_status = HTTPStatus.OK

                    if status:
                        response_json = {
                            'status': 1,
                            'error': ''
                        }

                    else:
                        response_json = {
                            'status': 0,
                            'error': err_str
                        }

                else:
                    response_json = {
                        'status': 0,
                        'error': 'Invalid username/password'
                    }
                    response_status = HTTPStatus.OK

            except RuntimeError as ex:
                self._logger.critical(ex)
                response_json = {
                    'status': 0,
                    'error': "Internal server error"
                }
                response_status = HTTPStatus.INTERNAL_SERVER_ERROR

        return Response(json.dumps(response_json), status = response_status,
                                   mimetype = mimetypes.types_map['.json'])
