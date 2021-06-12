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
from quart import request
from base_view import BaseView

class WebBaseView(BaseView):

    COOKIE_TOKEN = "items_token"
    COOKIE_USER = "items_user"

    def _generate_redirect(self, request, redirect_url) -> str:
        new_url = f"{request.url_root}{redirect_url}"
        return self.REDIRECT_URL.format(new_url)

    def _has_auth_cookies(self) -> bool:
        retrieved_token = request.cookies.get(self.COOKIE_TOKEN)
        retrieved_username = request.cookies.get(self.COOKIE_USER)
        return retrieved_token and retrieved_username

    def validate_cookies(self) -> bool:

        return True

        """
        retrieved_token = request.cookies.get(self.TOKEN_USER_TOKEN)
        retrieved_username = request.cookies.get(self.TOKEN_USER)

        if not retrieved_token or not retrieved_username:
            return False

        cookie = self._service.cookie_jar.get(CookieFieldType.USER_TOKEN,
                                              retrieved_token)

        if not cookie or cookie.username != retrieved_username:
            return False

        return True
        """
