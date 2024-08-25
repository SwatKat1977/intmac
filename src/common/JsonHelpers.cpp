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
#include "JsonHelpers.h"

std::string ValidateJsonAgainstSchema(const char* schema_json,
                                      const char* json_str) {
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
