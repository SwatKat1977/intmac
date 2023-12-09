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
#ifndef APIROUTE_H
#define APIROUTE_H
#include <memory>
#include <string>
#include <oatpp/web/server/HttpRequestHandler.hpp>

namespace items
{
    namespace serviceFramework
    {
        enum HTTPRequestMethod
        {
            // The GET method requests a representation of the specified
            // resource. Requests using GET should only retrieve data.
            HTTPRequestMethod_GET,

            // The HEAD method asks for a response identical to a GET request,
            // but without the response body.
            HTTPRequestMethod_HEAD,

            // The POST method submits an entity to the specified resource,
            // often causing a change in state or side effects on the server.
            HTTPRequestMethod_POST,

            // The PUT method replaces all current representations of the
            // target resource with the request payload.
            HTTPRequestMethod_PUT,

            // The DELETE method deletes the specified resource.
            HTTPRequestMethod_DELETE,

            // The CONNECT method establishes a tunnel to the server identified
            // by the target resource.
            HTTPRequestMethod_CONNECT,

            // The OPTIONS method describes the communication options for the
            // target resource.
            HTTPRequestMethod_OPTIONS,

            // The TRACE method performs a message loop - back test along the
            // path to the target resource.
            HTTPRequestMethod_TRACE,

            // The PATCH method applies partial modifications to a resource.
            HTTPRequestMethod_PATCH
        };

        using ApiOutResponse = oatpp::web::server::HttpRequestHandler::OutgoingResponse;
        using ApiOutResponsePtr = std::shared_ptr<ApiOutResponse>;
        using ApiIncomingReqPtr = std::shared_ptr<oatpp::web::server::HttpRequestHandler::IncomingRequest>;
        using ApiResponseStatus = oatpp::web::protocol::http::Status;
        using ApiResponseFactory = oatpp::web::protocol::http::outgoing::ResponseFactory;

        class ApiRoute
        {
        public:
            ApiRoute (std::string name) : m_name (name)
            {}

            std::string& Name () { return m_name; }

            virtual ApiOutResponsePtr Route (const ApiIncomingReqPtr &request) = 0;

        protected:
            std::string m_name;
        };

    }   // namespace serviceFramework
}   // namespace items

#endif
