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
#ifndef CONTROLLERS_ROOTCONTROLLER_H_
#define CONTROLLERS_ROOTCONTROLLER_H_
#include <memory>
#include <string>
#include <vector>
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "inja.hpp"
#include "WebRoute.h"

namespace items { namespace webPortalSvc { namespace controllers {

// Just for convenience
using namespace inja;

#include OATPP_CODEGEN_BEGIN(ApiController)

using serviceFramework::ApiResponseFactory;
using serviceFramework::ApiResponseStatus;

/*

class View(WebBaseView):
    ''' Home view container class. '''

    TEMPLATE_LOGIN_PAGE = "login.html"
    TEMPLATE_HOME_PAGE = "home.html"
    TEMPLATE_PROJECT_SELECTION_PAGE = "project_select.html"
    TEMPLATE_INTERNAL_ERROR_PAGE = "internal_server_error.html"

    def __init__(self, logger : logging.Logger):
        super().__init__()

        self._logger = logger.getChild(__name__)

        mimetypes.init()

    async def home_handler(self, api_request) -> Response:
        """
        Handler method for home page (e.g. '/').

        parameters:
            api_request - REST API request object

        returns:
            Instance of Quart Response class.
        """

        try:
            if not self._has_auth_cookies() or not self._validate_cookies():
                redirect = self._generate_redirect('login')
                return await make_response(redirect)

        except ItemsException as ex:
                self._logger.error('Internal Error: %s', ex)
                return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

        return await render_template(self.TEMPLATE_HOME_PAGE)

    async def login_handler(self, api_request) -> Response:
        """
        Handler method for login page.

        parameters:
            api_request - REST API request object

        returns:
            Instance of Quart Response class.
        """

        try:
            if self._has_auth_cookies() and self._validate_cookies():
                redirect = self._generate_redirect('')
                response = await make_response(redirect)
                return response

        except ItemsException as ex:
                self._logger.error('Internal Error: %s', ex)
                return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

        if api_request.method == "GET":
            return await render_template(self.TEMPLATE_LOGIN_PAGE)

        # If not a GET method, it can only be a POST, so handle that!
        user_email = (await api_request.form).get('user_email')
        password = (await api_request.form).get('password')

        if user_email and password:
            auth_body = {
                "email_address": user_email,
                "password": password
            }
            url = f"{Configuration().internal_api_gateway}/handshake/basic_authenticate"

            try:
                response = requests.post(url, json = auth_body)

            except requests.exceptions.ConnectionError as ex:
                self._logger.error('Connection to gateway api timed out: %s',
                                   str(ex))
                return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

            body = json.loads(response.content,
                              object_hook=lambda d: SimpleNamespace(**d))

            if response.status_code == HTTPStatus.NOT_ACCEPTABLE:
                except_str = ("Internal error communicating with gateway: "
                          f"{body.error}")
                self._logger.error(except_str)
                return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

            if body.status == 1:
                redirect = self._generate_redirect('')
                response = await make_response(redirect)
                response.set_cookie(self.COOKIE_USER, user_email)
                response.set_cookie(self.COOKIE_TOKEN, body.token)
                return response

            else:
                error_msg = "Invalid username/password"
                return await render_template(self.TEMPLATE_LOGIN_PAGE,
                                             generate_error_msg = True,
                                             error_msg = error_msg)

        return self._generate_redirect('/login')

    async def logout_handler(self, api_request):

        try:
            if not self._has_auth_cookies() or not self._validate_cookies():
                redirect = self._generate_redirect('login')
                return await make_response(redirect)

        except ItemsException as ex:
                self._logger.error('Internal Error: %s', ex)
                return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

        msg_body = {
            "email_address": request.cookies.get(self.COOKIE_USER),
            "token": request.cookies.get(self.COOKIE_TOKEN)
        }

        url = f"{self._config.gateway_api.base_url}/handshake/logout"

        try:
            response = requests.post(url, json = msg_body)

        except requests.exceptions.ConnectionError:
            return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

        if response.status_code == HTTPStatus.NOT_ACCEPTABLE:
            self._logger.error("Internal error communicating with gateway")
            return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

        redirect = self._generate_redirect('')
        response = await make_response(redirect)
        response.set_cookie(self.COOKIE_USER, '', expires = 0)
        response.set_cookie(self.COOKIE_TOKEN, '', expires = 0)

        return response
*/

const char TEMPLATE_LOGIN_PAGE[] = "login.html";
const char TEMPLATE_HOME_PAGE[] = "home.html";
const char TEMPLATE_INTERNAL_ERROR_PAGE[] = "internal_server_error.html";

class RootController : public WebRoute {
 public:
    RootController() : WebRoute() {}

    ENDPOINT("GET", "/", root,
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {

/*
        try:
            if not self._has_auth_cookies() or not self._validate_cookies():
                redirect = self._generate_redirect('login')
                return await make_response(redirect)

        except ItemsException as ex:
                self._logger.error('Internal Error: %s', ex)
                return await render_template(self.TEMPLATE_INTERNAL_ERROR_PAGE)

        return await render_template(self.TEMPLATE_HOME_PAGE)

*/

        auto cookieHeader = request->getHeader("Cookie");
        if (!cookieHeader) {
            auto response = ApiResponseFactory::createResponse(
                ApiResponseStatus::CODE_200, "revert to login placeholder A");
            response->putHeader("Content-Type", "text/html");
            return response;
        }

        printf("Cookie header : %s\n", cookieHeader.get()->c_str());
        std::vector<std::string> cookieValues = ParseCookieHeader(
            cookieHeader);

        if (!HasAuthCookies(cookieValues)) {
            auto response = ApiResponseFactory::createResponse(
                ApiResponseStatus::CODE_200, "revert to login placeholder B");
            response->putHeader("Content-Type", "text/html");
            return response;
        }

json data;
data["neighbour"] = "Peter";
data["guests"] = {"Jeff", "Tom", "Patrick"};

const char *test = "<table style=\"width:100%\">"
                   "{% for i in guests %}"
                   "<tr>"
     "<td>{{i}}</td>"
    "</tr>"
    "{% endfor %}"
    "</table>";

Environment env;

// Render a string with json data
std::string result = env.render(test, data); // "Hello world!"

std::cout << "RES : " << result << std::endl;

        return ApiResponseFactory::createResponse(
            ApiResponseStatus::CODE_200, "response");
    }
};

#include OATPP_CODEGEN_END(ApiController)

}   // namespace controllers
}   // namespace webPortalSvc
}   // namespace items

#endif  // CONTROLLERS_ROOTCONTROLLER_H_
