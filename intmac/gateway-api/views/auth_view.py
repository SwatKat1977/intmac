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
from quart import Blueprint, request

def create_auth_blueprint():
    auth_view = AuthView()

    auth_blueprint = Blueprint('auth_api', __name__)

    @auth_blueprint.route('/auth/authenticate', methods=['POST'])
    def authenticate_user_request():
        return auth_view.authenticate_user_handler(request)

    @auth_blueprint.route('/auth/logout', methods=['POST'])
    def logout_user_request():
        return auth_view.logout_user_handler(request)

    return auth_blueprint

class AuthView:

    def __init__(self):
        print('AuthView init()')

    def authenticate_user_handler(self, api_request):
        print('Test: Authenticate')
        return 'Test: Authenticate', 404

    def logout_user_handler(self, api_request):
        print('Test: Logout')
        return 'Test: Logout', 404
