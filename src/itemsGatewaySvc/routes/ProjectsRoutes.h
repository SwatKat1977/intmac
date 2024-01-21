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
-----------------------------------------------------------------------------
*/
#ifdef USE_PYTHON_CODE

#include "routes/TestcasesRoutes.h"

def create_blueprint(logger : logging.Logger):
    view = View(logger)

    blueprint = Blueprint('testcases_api', __name__)

    @blueprint.route('/internal/testcases/get_testsuites', methods=['GET'])
    async def get_testsuites_request():
        return await view.get_testsuites_request(request)

    @blueprint.route('/internal/testcases/get_testcases_overview', methods=['GET'])
    async def get_testcases_overview_request():
        return await view.get_testcases_overview_request(request)

    return blueprint

class View(BaseView):
    ''' Handshake view container class. '''

    GET_TESTSUITES_REQUEST_SCHEMA = \
    {
        '$schema': 'http://json-schema.org/draft-07/schema#',

        'type' : 'object',
        'additionalProperties' : False,

        'properties':
        {
            'project_id':
            {
                'type' : 'integer'
            }
        },
        'required' : ['project_id']
    }

    GET_TESTCASES_OVERVIEW_REQUEST_SCHEMA = \
    {
        '$schema': 'http://json-schema.org/draft-07/schema#',

        'type' : 'object',
        'additionalProperties' : False,

        'properties':
        {
            'project_id':
            {
                'type' : 'integer'
            }
        },
        'required' : ['project_id']
    }

    def __init__(self, logger : logging.Logger):
        self._logger : logging.Logger = logger.getChild(__name__)
        mimetypes.init()

    async def get_testsuites_request(self, request_entry : request) -> Response:
        """
        Handler method to get all testsuites for a given project.

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
                await request_entry.get_data(),
                self.GET_TESTSUITES_REQUEST_SCHEMA)

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
            Make API request from CMS service to get testsuite data.
            '''
            base_path : str = ThreadafeConfiguration().internal_apis_cms_svc

            request : dict = {
                "project_id": request_obj.body.project_id
            }
            url : str = (f'{base_path}'
                         '/testsuite/testsuites')

            api_response = await self._call_api_get(url, request)

            if api_response.status_code != HTTPStatus.OK:
                self._logger.critical("CMS svc request invalid - Reason: %s",
                                      api_response.body['error'])
                response_status = HTTPStatus.INTERNAL_SERVER_ERROR
                response_json = {
                    "status": 0,
                    'error': api_response.body['error']
                }

                return Response(json.dumps(response_json),
                                status=HTTPStatus.INTERNAL_SERVER_ERROR,
                                mimetype=mimetypes.types_map['.json'])

            response_status = HTTPStatus.OK

            response_json = {
                "data":  api_response.body
            }

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



    async def get_testcases_overview_request(self, request_entry : request) -> Response:
        """
        Handler method to get overview of testcases for a given project.

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
                await request_entry.get_data(),
                self.GET_TESTCASES_OVERVIEW_REQUEST_SCHEMA)

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
            Make API request from CMS service to get testsuite data.
            '''
            base_path : str = ThreadafeConfiguration().internal_apis_cms_svc

            request : dict = {
                "project_id": request_obj.body.project_id
            }
            url : str = (f'{base_path}'
                         '/testcase/testcases')

            api_response = await self._call_api_get(url, request)

            if api_response.status_code != HTTPStatus.OK:
                self._logger.critical("CMS svc request invalid - Reason: %s",
                                      api_response.body['error'])
                response_status = HTTPStatus.INTERNAL_SERVER_ERROR
                response_json = {
                    "status": 0,
                    'error': api_response.body['error']
                }

                return Response(json.dumps(response_json),
                                status=HTTPStatus.INTERNAL_SERVER_ERROR,
                                mimetype=mimetypes.types_map['.json'])

            response_status = HTTPStatus.OK

            response_json = {
                "data":  api_response.body
            }

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

#endif // #ifdef USE_PYTHON_CODE
