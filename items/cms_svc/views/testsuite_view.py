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
from base_view import ApiResponse, BaseView
from sqlite_interface import SqliteInterface

def create(logger : logging.Logger, db : SqliteInterface):
    view = TestsuiteView(logger, db)

    blueprint = Blueprint('testsuite_endpoint', __name__)

    @blueprint.route('/testsuite/testsuites', methods=['GET'])
    async def get_testsuites():
        return await view.get_testsuites()

    return blueprint

class TestsuiteView(BaseView):

    get_testsuites_schema = \
    {
        "$schema": "http://json-schema.org/draft-07/schema#",

        "type" : "object",
        "additionalProperties" : False,

        "properties":
        {
            "project_id":
            {
                "type" : "integer"
            }
        },
        "required" : ["project_id"]
    }

    def __init__(self, logger : logging.Logger, db : SqliteInterface):
        self._logger : logging.Logger = logger.getChild(__name__)
        self._db : SqliteInterface = db

        mimetypes.init()

    async def get_testsuites(self):

        request_obj : ApiResponse = self._validate_json_body(
            await request.get_data(), self.get_testsuites_schema)

        if request_obj.status_code != HTTPStatus.OK:
            response_json = {
                'status': 0,
                'error': request_obj.exception_msg
            }
            return Response(json.dumps(response_json),
                            status=HTTPStatus.INTERNAL_SERVER_ERROR,
                            mimetype=mimetypes.types_map['.json'])

        project_id : int = request_obj.body.project_id

        if not self._db.is_valid_project_id(project_id):
            response_json = {
                'status': 0,
                'error': "Invalid project id"
            }
            return Response(json.dumps(response_json),
                            status=HTTPStatus.INTERNAL_SERVER_ERROR,
                            mimetype=mimetypes.types_map['.json'])

        testsuites : list = self._db.get_testsuites_for_project(project_id)

        return Response(json.dumps(testsuites),
                        status=HTTPStatus.OK,
                        mimetype=mimetypes.types_map['.json'])
