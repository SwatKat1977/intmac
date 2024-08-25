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
#include <functional>
#include <iostream>
#include <sstream>
#include <rapidjson/document.h>
#include <rapidjson/schema.h>
#include <rapidjson/error/en.h>
#include <rapidjson/stringbuffer.h>
#include "ControllerDecorators.h"

//using namespace rapidjson;

std::string validate_json(const char* schema_json, const char* json_str) {
    // Parse the schema
    rapidjson::Document schema_doc;

    if (schema_doc.Parse(schema_json).HasParseError()) {
        std::stringstream str;
        str << "JSON Schema parsing error: "
            << GetParseError_En(schema_doc.GetParseError()) << " at "
            << schema_doc.GetErrorOffset();
        return str.str();
    }

    // Parse the JSON to validate
    rapidjson::Document json_doc;
    if (json_doc.Parse(json_str).HasParseError()) {
        std::stringstream str;
        str << "JSON parsing error: "
            << GetParseError_En(json_doc.GetParseError()) << " at "
            << json_doc.GetErrorOffset();
        return str.str();
    }

    // Create schema validator
    rapidjson::SchemaDocument schema(schema_doc);
    rapidjson::SchemaValidator validator(schema);

    // Validate the JSON
    if (!json_doc.Accept(validator)) {
        rapidjson::StringBuffer sb;
        std::stringstream str;
        validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
        str << "Invalid schema: " << sb.GetString() << "\n"
            << "\tInvalid keyword: "
            << validator.GetInvalidSchemaKeyword() << "\n";

        sb.Clear();
        validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
        str << "\tInvalid document: " << sb.GetString();
        return str.str();
    }

    return "";
}

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
        std::string res = validate_json(schema_json, json_str);
        if (!res.empty()) {
            std::cout << "[DEBUG]: " << res << "\n";
        }

    return 0;
}

namespace items { namespace webPortalSvc { namespace controllers {

/*
#ifdef TEST_
void validate_json(const json& schema, const json& json_obj) {
    try {
        json_validator validator;
        validator.set_root_schema(schema); // Parse and set the schema
        validator.validate(json_obj);       // Validate the JSON object

        std::cout << "JSON is valid according to the schema." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Validation failed: " << e.what() << std::endl;
    }
}

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

}   // namespace controllers
}   // namespace webPortalSvc
}   // namespace items
