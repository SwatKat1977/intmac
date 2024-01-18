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
#include "HandshakeRoutes.h"
#include "ConfigurationLayout.h"
#include "DTOs/HandshakeDTOs.h"
#include "Logger.h"
#include "UUID.h"

namespace items { namespace gatewaySvc {

    const std::string HEADERKEY_TOKEN = "auth_token";

    bool IsValidAuthToken (oatpp::String headerToken, std::string authToken)
    {
        if (!headerToken || headerToken->empty ())
        {
            return false;
        }

        return (headerToken == authToken);
    }

    BasicAuthenticate::BasicAuthenticate (
        std::string name,
        std::shared_ptr< common::apis::accountsSvc::AccountsSvcClient> accountsSvcClient,
        std::shared_ptr < SessionsManager> sessionManager)
        : ApiRoute (name),
          m_accountsSvcClient(accountsSvcClient),
          m_sessionManager(sessionManager)
    {
    }

    ApiOutResponsePtr BasicAuthenticate::Route (const ApiIncomingReqPtr& request)
    {
        auto response = BasicAuthenticateResponseDTO::createShared ();

        // ============== STEP 1 ==============
        // Validate the message body
        // ====================================
        auto requestMsg = request->readBodyToDto<oatpp::Object
            <BasicAuthenticateRequestDTO>> (m_objectMapper.get ());

        if ((!requestMsg.get ()->email_address) ||
            (!requestMsg.get ()->password))
        {
            response->status = BASIC_AUTH_RESPONSE_STATUS_BAD;
            response->error = "Invalid request format";
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_200, response,
                m_objectMapper);
        }

        std::string emailAddress = requestMsg.get ()->email_address;
        std::string password = requestMsg.get ()->password;

        // ============== STEP 2 ==============
        // Request authentication verification from accounts service.
        // ====================================
        auto requestBody = common::apis::accountsSvc::AccountsSvcBasicAuthRequestDto::createShared ();
        requestBody->email_address = emailAddress;
        requestBody->password = requestMsg.get ()->password;
        
        oatpp::Object<common::apis::accountsSvc::AccountsSvcBasicAuthResponseDto> authResponse;

        try
        {
            authResponse = m_accountsSvcClient
                ->basicAuthenticateDTO (requestBody)
                ->readBodyToDto<oatpp::Object<common::apis::accountsSvc::AccountsSvcBasicAuthResponseDto>> (m_objectMapper.get ());
        }
        catch (std::runtime_error& ex)
        {
            LOGGER->error ("Cannot connect to accounts service API, reason: {0}",
                ex.what ());
            response->status = BASIC_AUTH_RESPONSE_STATUS_BAD; 
            response->error = "Internal error";
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_200, response,
                m_objectMapper);
        }

        if (authResponse->status == BASIC_AUTH_RESPONSE_STATUS_BAD)
        {
            response->status = BASIC_AUTH_RESPONSE_STATUS_BAD;
            response->error = "Invalid username/password";
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_200, response,
                m_objectMapper);
        }

        std::string token = common::UUID::New ().ToString ();

        bool hasSession = m_sessionManager->HasSession (emailAddress);

        m_sessionManager->AddSession (emailAddress, token, AuthenticationType_MANUAL);

        std::string loginType = hasSession ? "re-logged in" : "logged in";
        LOGGER->info ("User '{0}' {1}", emailAddress, loginType);

        response->status = BASIC_AUTH_RESPONSE_STATUS_OK;
        response->error = "";
        return ApiResponseFactory::createResponse (
            ApiResponseStatus::CODE_200, response,
            m_objectMapper);
    }

    Logout::Logout (
        std::string name,
        std::shared_ptr < SessionsManager> sessionManager,
        serviceFramework::ConfigManager configManager)
        : ApiRoute (name), m_sessionManager (sessionManager),
        m_configManager(configManager)
    {
    }

    ApiOutResponsePtr Logout::Route (const ApiIncomingReqPtr& request)
    {
        auto headerToken = request->getHeader (HEADERKEY_TOKEN.c_str());

        std::string authToken = m_configManager.GetStringEntry (
            SECTION_AUTHENTICATION, AUTHENTICATION_TOKEN);

        if (!IsValidAuthToken (headerToken, authToken))
        {
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_401, "voooo");
        }

        if (!headerToken || headerToken->empty ())
        {
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_401, "");
        }

        std::string authToken2 = m_configManager.GetStringEntry (
            SECTION_AUTHENTICATION, AUTHENTICATION_TOKEN);

        if (headerToken != authToken)
        {
            return ApiResponseFactory::createResponse (
                ApiResponseStatus::CODE_401, "");
        }

        return ApiResponseFactory::createResponse (
            ApiResponseStatus::CODE_200, "response");

#ifdef __OLD_PYTHON_CODE__
        request_obj, err_msg = await self._convert_json_body_to_object (
            api_request, self.logoutRequestSchema)

        if not request_obj:
            self._logger.error ("Received bad logout request")
            response = "BAD REQUEST"
            response_status = HTTPStatus.NOT_ACCEPTABLE

        else:
            if self._sessions.is_valid_session (request_obj.email_address,
                request_obj.token) :
                self._sessions.del_auth_session (request_obj.email_address)
                self._logger.info ("User '%s' logged out",
                    request_obj.email_address)

        response = "OK"
        response_status = HTTPStatus.OK

        return Response (response, status = response_status,
            mimetype = mimetypes.types_map['.txt'])
#endif
    }

#ifdef __OLD_PYTHON_CODE__
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

#endif

    }
}   // namespace items::gatewaySvc


#ifdef __OLD_PYTHON_CODE__
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

#ifdef __OLD_PYTHON_CODE__
tests = [
{
    'id': 0,
        'name' : b'root',
        'parent' : -1,
        'type' : 'filter'
},
    {
        'id': 1,
        'name' : b'UI Tests',
        'parent' : 0,
        'type' : 'filter'
    },
    {
        'id': 2,
        'name' : b'API Tests',
        'parent' : 0,
        'type' : 'filter'
    },
    {
        'id': 3,
        'name' : b'Items Login Successful',
        'parent' : 2,
        'type' : 'test case'
    },
    {
        'id': 4,
        'name' : b'Items Login Unsuccessful',
        'parent' : 2,
        'type' : 'test case'
    }
]

test_sets = [
    {
        'id': 0,
            'name' : b'root',
            'parent' : -1,
            'type' : 'filter'
    },
    {
        'id': 0,
        'name' : b'RELEASE_1.0.0',
        'parent' : 0,
        'type' : 'filter'
    },
    {
        'id': 0,
        'name' : b'RELEASE_1.0.1',
        'parent' : 0,
        'type' : 'filter'
    },
]
#endif

#ifdef __OLD_PYTHON_CODE__

            @blueprint.route ('/handshake/logout', methods = ['POST'])
            async def logout_user_request () :
            # pylint : disable = unused - variable
            return await view.logout_user_handler (request)

            @blueprint.route ('/handshake/valid_token', methods = ['GET'])
            async def valid_token_request () :
            # pylint : disable = unused - variable
            return await view.valid_token_handler (request)

            @blueprint.route ('/handshake/get_projects', methods = ['GET'])
            async def get_projects_request () :
            # pylint : disable = unused - variable
            return await view.get_projects_handler (request)

            @blueprint.route ('/handshake/select_project', methods = ['POST'])
            async def select_project_request () :
            # pylint : disable = unused - variable
            return await view.select_projects_handler (request)

            return blueprint

            class View (BaseView) :
            ''' Handshake view container class. '''

            basicAuthenticateRequestSchema = \
        {
            "$schema": "http://json-schema.org/draft-07/schema#",

                "type" : "object",
                "additionalProperties" : False,

                "properties" :
            {
                "email_address":
                {
                    "type" : "string"
                },
                    "password" :
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

                "properties" :
            {
                "email_address":
                {
                    "type" : "string"
                },
                    "token" :
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

                "properties" :
            {
                "email_address":
                {
                    "type" : "string"
                },
                    "token" :
                    {
                        "type" : "string"
                    },
            },
                "required" : ["email_address", "token"]
        }

            async def valid_token_handler (self, api_request) :
            """
            Endpoint to check to see if a token is valid.

           parameters :
            api_request - Request from Quart

            returns :
        Response instance
            """

            request_obj : ApiResponse = self._validate_json_body (
                await api_request.get_data (), self.tokenValidationCheckRequestSchema)

            if request_obj.status_code != HTTPStatus.OK :
                response_json = { 'status': 'BAD REQUEST' }
                return Response (json.dumps (response_json),
                    status = HTTPStatus.NOT_ACCEPTABLE,
                    mimetype = mimetypes.types_map['.json'])

                valid = self._sessions.is_valid_session (request_obj.body.email_address,
                    request_obj.body.token)
                response_json = { "status": "VALID" if valid else "INVALID" }
                response_status = HTTPStatus.OK

                return Response (json.dumps (response_json), response_status,
                    mimetype = mimetypes.types_map['.json'])

                async def get_projects_handler (self, api_request) :
                """ PLACEHOLDER """
                # pylint : disable = unused - argument

                self._logger.debug ("getprojects currently returns a hard-coded list")

                response = {
                    "projects": [
                        {
                            "name": "test project #1",
                            "description" : "This is test project #1"
                        },
                        {
                            "name": "test project #2",
                            "description" : "This is test project #2"
                        }
                    ]
            }

                return Response (json.dumps (response), status = HTTPStatus.OK,
                    mimetype = mimetypes.types_map['.json'])

                                async def select_projects_handler (self, api_request) :
                                """ PLACEHOLDER """
                                # pylint : disable = unused - argument

                                self._logger.debug ("selectprojects returns a hard-coded response")

                                def generate () :
                                yield b'{'

                                try :
                                tests_iter = tests.__iter__ ()

                                previous = next (tests_iter)

                                yield b'"test_cases": ['

                                for entry in tests_iter :
                            name = previous.get ('name')

                                yield (
                                    "{"
                                    f"\"id\":{previous.get('id')}, "
                                    "\"name\":"
                                    "\"" + b64encode (name).decode ('ascii') + "\","
                                    f"\"parent\":{previous.get('parent')}, "
                                    f"\"type\":\"{previous.get('type')}\""
                                    "},"
                                ).encode ("UTF-8")
                                previous = entry

                                yield (
                                    "{"
                                    f"\"id\":{previous.get('id')}, "
                                    "\"name\":"
                                    "\"" + b64encode (name).decode ('ascii') + "\","
                                    f"\"parent\":{previous.get('parent')}, "
                                    f"\"type\":\"{previous.get('type')}\""
                                    "}],"
                                ).encode ("UTF-8")

                                except StopIteration :
                            yield b'"test_cases": [],'

                                try :
                                test_sets_iter = test_sets.__iter__ ()

                                previous = next (test_sets_iter)

                                yield b'"test_sets": ['

                                for entry in test_sets_iter :
                            name = previous.get ('name')

                                yield (
                                    "{"
                                    f"\"id\":{previous.get('id')}, "
                                    "\"name\":"
                                    "\"" + b64encode (name).decode ('ascii') + "\","
                                    f"\"parent\":{previous.get('parent')}, "
                                    f"\"type\":\"{previous.get('type')}\""
                                    "},"
                                ).encode ("UTF-8")
                                previous = entry

                                yield (
                                    "{"
                                    f"\"id\":{previous.get('id')}, "
                                    "\"name\":"
                                    "\"" + b64encode (name).decode ('ascii') + "\","
                                    f"\"parent\":{previous.get('parent')}, "
                                    f"\"type\":\"{previous.get('type')}\""
                                    "}]"
                                ).encode ("UTF-8")

                                except StopIteration :
                            yield b'"test_sets": []'

                                yield b'}'

                                return Response (generate (), status = HTTPStatus.OK,
                                    content_type = 'application/json')

#endif
