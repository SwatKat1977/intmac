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
from sqlite_interface import SqliteInterface
from quart import Blueprint, request

def create_basic_auth_blueprint(sql_interface : SqliteInterface):
    view = View(sql_interface)

    blueprint = Blueprint('basic_auth_api', __name__)

    @blueprint.route('/basic_auth/authenticate', methods=['POST'])
    def authenticate_request():
        return view.authenticate(request)

    return blueprint

class View:
    __slots__ = ['_sql_interface']

    def __init__(self, sql_interface : SqliteInterface) -> None:
        self._sql_interface = sql_interface
 
    def authenticate(self, api_request):
        print('Test: Placeholder')
        return 'Test: Placeholder', 404
