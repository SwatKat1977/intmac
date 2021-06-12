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
import logging
import mimetypes
from quart import Blueprint, request, Response
from base_view import BaseView
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING

def create_home_blueprint():
    view = View()

    blueprint = Blueprint('home', __name__)

    @blueprint.route('/', methods=['GET'])
    async def home_request():
        # pylint: disable=unused-variable
        return await view.home_handler(request)

    return blueprint

class View(BaseView):
    ''' Home view container class. '''

    def __init__(self):
        self._logger = logging.getLogger(__name__)
        log_format= logging.Formatter(LOGGING_LOG_FORMAT_STRING,
                                      LOGGING_DATETIME_FORMAT_STRING)
        console_stream = logging.StreamHandler()
        console_stream.setFormatter(log_format)
        self._logger.setLevel(LOGGING_DEFAULT_LOG_LEVEL)
        self._logger.addHandler(console_stream)

        mimetypes.init()

    async def home_handler(self, api_request) -> Response:
        """
        Handler method for basic user authentication endpoint.

        parameters:
            api_request - REST API request object

        returns:
            Instance of Quart Response class.
        """

        return Response('ok',
                        status = HTTPStatus.OK,
                        mimetype = mimetypes.types_map['.txt']
                       )
