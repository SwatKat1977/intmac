/*
-----------------------------------------------------------------------------
This source file is part of ITEMS
(Integrated Test Management Suite)
For the latest info, see https://github.com/SwatKat1977/intmac/

Copyright 2014-2024 Integrated Test Management Suite Development Team

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

The following is based on Ogre3D code:
* GetEnv from os-int.h
-----------------------------------------------------------------------------
*/
#include "ServiceContext.h"
#include "apis/accountsSvc/AccountsSvcClient.h"
#include "SessionsManager.h"

namespace items { namespace gatewaySvc {

    using namespace serviceFramework;

    class BasicAuthenticate : public ApiRoute
    {
    public:
        BasicAuthenticate (
            std::string name,
            std::shared_ptr< common::apis::accountsSvc::AccountsSvcClient> accountsSvcClient,
            std::shared_ptr < SessionsManager> sessionManager);

        ApiOutResponsePtr Route (const ApiIncomingReqPtr& request);

    private:
        std::shared_ptr< common::apis::accountsSvc::AccountsSvcClient> m_accountsSvcClient;
        std::shared_ptr < SessionsManager> m_sessionManager;
    };

    class Logout : public ApiRoute
    {
    public:
        Logout (
            std::string name,
            std::shared_ptr < SessionsManager> sessionManager,
            serviceFramework::ConfigManager configManager);

        ApiOutResponsePtr Route (const ApiIncomingReqPtr& request);

    private:
        std::shared_ptr < SessionsManager> m_sessionManager;
        serviceFramework::ConfigManager m_configManager;
    };

    class IsValidToken : public ApiRoute
    {
    public:
        IsValidToken (std::string name);

        ApiOutResponsePtr Route (const ApiIncomingReqPtr& request);
    };

    class GetProjects : public ApiRoute
    {
    public:
        GetProjects (std::string name);

        ApiOutResponsePtr Route (const ApiIncomingReqPtr& request);
    };

    class SelectProject : public ApiRoute
    {
    public:
        SelectProject (std::string name);

        ApiOutResponsePtr Route (const ApiIncomingReqPtr& request);
    };

} }   // namespace items::gatewaySvc


#ifdef __OldCode__
from base64 import b64encode
from http import HTTPStatus
import json
import logging
import mimetypes
import uuid
from quart import Blueprint, request, Response
import requests
from base_view import ApiResponse, BaseView
from threadsafe_configuration import ThreadafeConfiguration
from logon_type import LogonType
from redis_interface import RedisInterface
#endif

#ifdef __OldCode__
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
#endif

#ifdef __OldCode__
def create_handshake_blueprint(sessions : RedisInterface,
                               logger : logging.Logger):
    view = View(sessions, logger)

    blueprint = Blueprint('handshake_api', __name__)

    @blueprint.route('/handshake/basic_authenticate', methods=['POST'])
    async def basic_authenticate_request():
        # pylint: disable=unused-variable
        return await view.basic_authenticate_handler(request)

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

    logoutRequestSchema = \
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

    def __init__(self, sessions : RedisInterface, logger : logging.Logger):
        self._sessions = sessions

        self._logger : logging.Logger = logger.getChild(__name__)

        mimetypes.init()

    async def basic_authenticate_handler(self, api_request) -> Response:
        """
        Handler method for basic user authentication endpoint.

        parameters:
            api_request - REST API request object

        returns:
            Instance of Quart Response class.
        """

        try:
            '''
            STEP 1:
            Validate the message body:
            1) Is JSON format
            2) Validates against JSON schema
            '''
            request_obj : ApiResponse = self._validate_json_body(
                await api_request.get_data(), self.basicAuthenticateRequestSchema)

            if request_obj.status_code != HTTPStatus.OK:
                response_json = {
                    'status': 'BAD',
                    'error': request_obj.exception_msg
                }
                return Response(json.dumps(response_json),
                                status=HTTPStatus.INTERNAL_SERVER_ERROR,
                                mimetype=mimetypes.types_map['.json'])

            '''
            STEP 2:
            Request authentication verification from accounts service.
            '''
            base_accounts_svc : str = ThreadafeConfiguration().get_entry(
                "internal_apis", "accounts_svc")

            auth_request : dict = {
                "email_address": request_obj.body.email_address,
                "password": request_obj.body.password
            }
            auth_url : str = (f"{base_accounts_svc}"
                              "/basic_auth/authenticate")

            response = await self._call_api_post(auth_url, auth_request)

            if response.status_code != HTTPStatus.OK:
                self._logger.critical("Accounts svc request invalid - Reason: %s",
                                      response.exception_msg)
                response_status = HTTPStatus.INTERNAL_SERVER_ERROR
                response_json = {
                    "status": 'BAD',
                    'error': 'Internal error!'
                }
                return Response(json.dumps(response_json),
                                status=HTTPStatus.INTERNAL_SERVER_ERROR,
                                mimetype=mimetypes.types_map['.json'])

            response_status = HTTPStatus.OK

            if not response.body.get("status"):
                response_json = {
                    "status":  0,
                    "error": response.body.get("error")
                }

            else:
                token = uuid.uuid4().hex
                self._sessions.add_auth_session(
                    request_obj.body.email_address, token, LogonType.BASIC)

                response_json = {
                    "status": 1,
                    "token": token
                }
                self._logger.info("User '%s' logged in",
                                  request_obj.body.email_address)

        except requests.exceptions.ConnectionError as ex:
            except_str = f"Internal error: {ex}"
            self._logger.error(except_str)

            response_json = {
                "status":  0,
                "error": str(ex)
            }
            response_status = HTTPStatus.INTERNAL_SERVER_ERROR

        return Response(json.dumps(response_json), status = response_status,
                                   mimetype = mimetypes.types_map['.json'])

    async def logout_user_handler(self, api_request) -> Response:
        """
        Handler method for user session logout endpoint.

        parameters:
            api_request - REST API request object

        returns:
            Instance of Quart Response class.
        """

        request_obj, err_msg = await self._convert_json_body_to_object(
            api_request, self.logoutRequestSchema)

        if not request_obj:
            self._logger.error("Received bad logout request")
            response =  "BAD REQUEST"
            response_status = HTTPStatus.NOT_ACCEPTABLE

        else:
            if self._sessions.is_valid_session(request_obj.email_address,
                                               request_obj.token):
                self._sessions.del_auth_session(request_obj.email_address)
                self._logger.info("User '%s' logged out",
                                  request_obj.email_address)

            response =  "OK"
            response_status = HTTPStatus.OK

        return Response(response, status=response_status,
                        mimetype=mimetypes.types_map['.txt'])

    async def valid_token_handler(self, api_request):
        """
        Endpoint to check to see if a token is valid.

        parameters:
            api_request - Request from Quart

        returns:
            Response instance
        """

        request_obj : ApiResponse = self._validate_json_body(
            await api_request.get_data(), self.tokenValidationCheckRequestSchema)

        if request_obj.status_code != HTTPStatus.OK:
            response_json = { 'status': 'BAD REQUEST' }
            return Response(json.dumps(response_json),
                            status=HTTPStatus.NOT_ACCEPTABLE,
                            mimetype=mimetypes.types_map['.json'])

        valid = self._sessions.is_valid_session(request_obj.body.email_address,
                                                request_obj.body.token)
        response_json = { "status": "VALID" if valid else "INVALID" }
        response_status = HTTPStatus.OK

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

#endif
