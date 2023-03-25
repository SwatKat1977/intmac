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
from dataclasses import dataclass
import json
from types import SimpleNamespace
from typing import Optional, Tuple, Union
import aiohttp
import jsonschema

@dataclass(init=True)
class ApiResponse:
    """ Class for keeping track of api return data. """
    status_code: int
    body: dict | str
    contents_type : str
    exception_msg : str

    def __init__(self,
                 status_code: int = 0,
                 body: dict | str = None,
                 contents_type : str = None,
                 exception_msg : str = None):
        self.status_code = status_code
        self.body = body
        self.contents_type = contents_type
        self.exception_msg = exception_msg

class BaseView:
    """ Base view class """

    async def _convert_json_body_to_object(self, request_instance,
                                           json_schema = None) -> \
              Tuple[Union[str, None], str]:
        """
        Convert JSON  Validate the authentication key for a request.

        parameters:
            equest_instance - Instance of a message request to be converted.\n
            json_schema - Optional Json schema to validate the body against.

        returns:
            object, error_string.  If successful then object is a valid object
            and error_string is empty. On failure the object is None and
            error_string is set to an appropriate error message.
        """

        # Check for that the message body is of type application/json and that
        # there is one, if not report a 400 error status with a human-readable.
        body = await request_instance.get_json()
        if not body:
            return None, 'Missing/invalid json body'

        if json_schema:
            # Validate that the json body conforms to the expected schema.  If
            # the body isn't valid then an error should be generated and object
            # returned as None.
            try:
                jsonschema.validate(instance=body, schema=json_schema)

            except jsonschema.exceptions.ValidationError:
                return None, 'Message body validation failed.'

        # Parse JSON into an object with attributes corresponding to dict keys.
        obj_instance = json.loads(await request_instance.get_data(),
                                  object_hook=lambda d: SimpleNamespace(**d))
        return obj_instance, ''

    async def _call_api_post(self, url : str, json_data : dict = None) -> ApiResponse:
        """
        Make an API call using the POST method.

        parameters:
            url - URL of the endpoint
            json_data - Optional Json body.

        returns:
            ApiResponse which will will contain response data or just
            exception_msg if something went wrong.
        """

        try:
            async with aiohttp.ClientSession() as session:
                async with session.post(url, json=json_data) as resp:
                    body = await resp.json() \
                        if resp.content_type == 'application/json' \
                        else await resp.text()
                    api_return = ApiResponse(
                        status_code = resp.status,
                        body = body,
                        contents_type = resp.content_type)

        except Exception as ex:
            api_return = ApiResponse(exception_msg = ex)

        return api_return

    async def _call_api_get(self, url : str, json_data : dict = None) -> ApiResponse:
        """
        Make an API call using the GET method.

        parameters:
            url - URL of the endpoint
            json_data - Optional Json body.

        returns:
            ApiResponse which will will contain response data or just
            exception_msg if something went wrong.
        """

        try:
            async with aiohttp.ClientSession() as session:
                async with session.get(url, json=json_data) as resp:
                    body = await resp.json() \
                        if resp.content_type == 'application/json' \
                        else await resp.text()
                    api_return = ApiResponse(
                        status_code = resp.status,
                        body = body,
                        contents_type = resp.content_type)

        except Exception as ex:
            api_return = ApiResponse(exception_msg = ex)

        return api_return
