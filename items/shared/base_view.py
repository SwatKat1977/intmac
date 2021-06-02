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
import json
from types import SimpleNamespace
from typing import Tuple, Union
import jsonschema

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
