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

def create_placeholder_blueprint():
    view = PlaceholderView()

    blueprint = Blueprint('placeholder_api', __name__)

    @blueprint.route('/PLACEHOLDER', methods=['POST'])
    def authenticate_user_request():
        return view.placeholder_handler(request)

    return blueprint

class PlaceholderView:

    def __init__(self):
        print('AuthView init()')

    def placeholder_handler(self, api_request):
        print('Test: Placeholder')
        return 'Test: Placeholder', 402
