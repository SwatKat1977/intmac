/*
-----------------------------------------------------------------------------
This source file is part of ITEMS
(Integrated Test Management Suite)
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
#include <sstream>
#include "WebRoute.h"
#include "Logger.h"
#include "apis/gatewaySvc/GatewaySvcClient.h"

namespace items { namespace webPortalSvc {

    // Name of cookies.
    const std::string COOKIE_TOKEN = "items_token";
    const std::string COOKIE_USER = "items_user";

    const std::string REDIRECT_URL = "<meta http-equiv=\"Refresh\" content=\"0; url='{0}\"/>";

    WebRoute::WebRoute (std::string name) : ApiRoute(name)
    {
    }

    std::string WebRoute::GenerateRedirect (std::string redirectURLRoot,
        std::string redirectURL)
    {
        std::stringstream url;
        url << "<meta http-equiv=\"Refresh\" content=\"0; url='"
            << redirectURLRoot << redirectURL << "'\"/>";
        return url.str ();
    }

    void WebRoute::GetAuthCookies (
        std::vector<std::string> cookies,
        std::string& tokenValue,
        std::string& userValue)
    {
        for (const auto& cookieEntry : cookies)
        {
            std::string cookieKey;
            std::string cookieValue;

            if (!GetCookieValue (cookieEntry, cookieKey, cookieValue))
            {
                continue;
            }

            if (cookieKey == COOKIE_TOKEN) tokenValue = cookieValue;
            if (cookieKey == COOKIE_USER) userValue = cookieValue;
        }
    }

    bool WebRoute::HasAuthCookies (std::vector<std::string> cookies)
    {
        std::string cookieTokenValue;
        std::string cookieUserValue;

        GetAuthCookies (cookies, cookieTokenValue, cookieUserValue);

        if (cookieTokenValue.empty () || cookieUserValue.empty ())
        {
            return false;
        }

        printf ("COOKIE token : %s\n", cookieTokenValue.c_str ());
        printf ("COOKIE user  : %s\n", cookieUserValue.c_str ());

        return true;
    }

    bool WebRoute::ValidAuthCookies (
        std::string tokenValue, std::string userValue)
    {
        bool returnStatus = false;

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

        url = f"{Configuration().internal_api_gateway}/handshake/valid_token"

        request_body = {
            "email_address": username,
            "token" : token
        }

        try
        {
            response = requests.get (url, json = request_body, timeout = 1)
        {
        except (requests.exceptions.ConnectionError as ex)
        {
            raise ItemsException ('Connection to gateway api timed out') from ex
        }

        data = json.loads (response.content,
            object_hook = lambda d : SimpleNamespace (**d))

        if response.status_code == HTTPStatus.NOT_ACCEPTABLE:
    except_str = ("Internal error communicating with gateway: "
        f"{data.status}")
        raise ItemsException (except_str)

        if data.status == "VALID" :
            return_status = True

        return return_status
    }

    std::vector<std::string> WebRoute::ParseCookieHeader (const std::string& cookieHeader)
    {
        std::vector<std::string> cookieValues;
        std::istringstream iss (cookieHeader);
        std::string cookieValue;

        while (std::getline (iss, cookieValue, ';'))
        {
            LeftTrim (cookieValue);
            cookieValues.push_back (cookieValue);
        }
        return cookieValues;
    }

    bool WebRoute::GetCookieValue (const std::string& cookieValue,
                                   std::string& name,
                                   std::string& value)
    {
        size_t delimiterPos = cookieValue.find ('=');
        if (delimiterPos != std::string::npos)
        {
            name = cookieValue.substr (0, delimiterPos);
            value = cookieValue.substr (delimiterPos + 1);
            return true;
        }
        return false;
    }

} }   // namespace items::webPortalSvc
