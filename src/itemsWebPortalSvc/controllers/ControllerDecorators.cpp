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
#include <iostream>
#include "JsonHelpers.h"
#include "ControllerDecorators.h"

int main() {
    // Define a JSON schema
    const char* schema_json = R"({
        "type": "object",
        "properties": {
            "name": { "type": "string" },
            "age": { "type": "integer" },
            "email": { "type": "string", "format": "email" }
        },
        "required": ["name", "age"]
    })";

    // Define a JSON object that should be validated
    const char* json_str = R"({
        "name": "John Doe",
        "age": "30",
        "email": "john.doe@example.com"
    })";

        // Validate the JSON object against the schema
        std::string res = items::ValidateJsonAgainstSchema(schema_json, json_str);
        if (!res.empty()) {
            std::cout << "[DEBUG]: " << res << "\n";
        }

    return 0;
}

namespace items { namespace webPortalSvc { namespace controllers {

/*
#ifdef TEST_


def validate_json_body(json_schema : dict = None):
    """
    Decorator to validate the JSON body of a request.

    Args:
        json_schema (dict, optional): The JSON schema to validate against. Defaults to None.

    Returns:
        function: The decorated function with JSON body validation.
    """

    def decorator(func):
        @functools.wraps(func)
        async def wrapper(*args, **kwargs):
            """
            Wrapper function that performs the JSON body validation.

            Args:
                *args: Positional arguments passed to the wrapped function.
                **kwargs: Keyword arguments passed to the wrapped function.

            Returns:
                Response: A Flask response object indicating the validation result.
            """

            #body = await request.get_json()
            body = await request.body
            if body is None:
                return Response(ERR_MSG_MISSING_INVALID_JSON_BODY,
                                status=http.HTTPStatus.BAD_REQUEST,
                                content_type=ERR_MSG_CONTENT_TYPE)

            try:
                json_data = json.loads(body)

            except (TypeError, json.JSONDecodeError) as ex:
                print(ex)
                return Response(ERR_MSG_INVALID_BODY_TYPE,
                                status=http.HTTPStatus.BAD_REQUEST,
                                content_type=ERR_MSG_CONTENT_TYPE)

            # If there is a JSON schema then validate that the json body conforms
            # to the expected schema. If the body isn't valid then a 400 error
            # should be generated.
            if json_schema:
                try:
                    jsonschema.validate(instance=json_data,
                                        schema=json_schema)

                except jsonschema.exceptions.ValidationError:
                    return Response(ERR_MSG_BODY_SCHEMA_MISMATCH,
                                    status=http.HTTPStatus.BAD_REQUEST,
                                    content_type=ERR_MSG_CONTENT_TYPE)

            return await func(*args, **kwargs)

        return wrapper

    return decorator
#endif
*/

/*

def validate_auth_key(auth_key_key: str, configuration):
    """
    Decorator to validate the authorization key in the request header.

    Args:
        auth_key_key (str): The name of the authorization header key.
        auth_key_value (str): The expected value of the authorization key.

    Returns:
        function: The decorated function which includes authorization key validation.
    """

    def decorator(func):
        """
        Wrapper function to validate the authorization key.

        Returns:
            Response: HTTP response with 401 if the authorization key is missing,
                      403 if the authorization key is invalid, or
                      the result of the decorated function if the key is valid.
        """

        @functools.wraps(func)
        async def wrapper(*args, **kwargs):

            # Verify that an authorisation key exists in the request header, if
            # not then return a 401 error with a human-readable reasoning.
            if auth_key_key not in request.headers:
                return Response(ERR_MSG_AUTH_KEY_MISSING,
                                status=http.HTTPStatus.UNAUTHORIZED,
                                content_type=ERR_MSG_CONTENT_TYPE)

            # Verify the authorisation key against what is specified in the
            # configuration file.  If the key isn't valid then the error
            # code of 403 (Forbidden) is returned.
            if configuration.general_authentication_key != request.headers[auth_key_key]:
                return Response(ERR_MSG_AUTH_KEY_INVALID,
                                status=http.HTTPStatus.FORBIDDEN,
                                content_type=ERR_MSG_CONTENT_TYPE)

            return await func(*args, **kwargs)

        return wrapper

    return decorator
*/

}   // namespace controllers
}   // namespace webPortalSvc
}   // namespace items
