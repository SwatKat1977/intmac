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
from http import HTTPStatus
import json
from types import SimpleNamespace
from quart import request
import requests
from base_view import BaseView
from config import Config
from items_exception import ItemsException

class WebBaseView(BaseView):
    """ Base class for views that serve web pages """
    # pylint: disable=too-few-public-methods

    COOKIE_TOKEN = "items_token"
    COOKIE_USER = "items_user"

    REDIRECT_URL = "<meta http-equiv=\"Refresh\" content=\"0; url='{0}\"/>"

    def __init__(self, config : Config) -> None:
        super().__init__()

        self._config = config

    def _generate_redirect(self, redirect_url) -> str:
        new_url = f"{request.url_root}{redirect_url}"
        return self.REDIRECT_URL.format(new_url)

    def _has_auth_cookies(self) -> bool:
        retrieved_token = request.cookies.get(self.COOKIE_TOKEN)
        retrieved_username = request.cookies.get(self.COOKIE_USER)
        return retrieved_token and retrieved_username

    def _validate_cookies(self) -> bool:
        return_status = False

        token = request.cookies.get(self.COOKIE_TOKEN)
        username = request.cookies.get(self.COOKIE_USER)

        url = f"{self._config.gateway_api.base_url}/handshake/valid_token"

        request_body = {
            "email_address": username,
            "token": token
        }

        try:
            response = requests.get(url, json = request_body)

        except requests.exceptions.ConnectionError as ex:
            raise ItemsException('Connection to gateway api timed out') from ex

        data = json.loads(response.content,
                          object_hook=lambda d: SimpleNamespace(**d))

        if response.status_code == HTTPStatus.NOT_ACCEPTABLE:
            except_str = ("Internal error communicating with gateway: "
                          f"{data.status}")
            raise ItemsException(except_str)

        if data.status == "VALID":
            return_status = True

        return return_status
