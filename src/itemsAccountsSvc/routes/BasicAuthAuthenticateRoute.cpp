/*
-----------------------------------------------------------------------------
This source file is part of ITEMS
(Integrated Test Management Suite )
For the latest info, see https://github.com/SwatKat1977/intmac/

Copyright 2014-2023 Integrated Test Management Suite Development Team

    This program is free software : you can redistribute it and /or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see < https://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
*/
#ifdef ___PYTHON___
from http import HTTPStatus
import json
import logging
import mimetypes
from quart import Blueprint, request, Response
from base_view import ApiResponse, BaseView
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

        response : ApiResponse = self._validate_json_body(
            await api_request.get_data(), AuthenticateRequest.schema)

        if response.status_code != HTTPStatus.OK:
            response_json = {
                'status': 0,
                'error': response.exception_msg
            }
            return Response(json.dumps(response_json),
                            status=HTTPStatus.INTERNAL_SERVER_ERROR,
                            mimetype=mimetypes.types_map['.json'])

        try:
            user_id, _ = self._sql_interface.valid_user_to_logon(
                response.body.email_address, LogonType.BASIC)

            if user_id:
                status, err_str = self._sql_interface.basic_user_authenticate(
                    user_id, response.body.password)

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

#endif
