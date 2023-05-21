'''
Copyright 2014-2023 Integrated Test Management Suite

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
import logging
from quart import Blueprint, request

def create(logger : logging.Logger):
    view = TestsuiteView(logger)

    blueprint = Blueprint('testcase_api', __name__)

    @blueprint.route('/testsuites/testsuites', methods=['GET'])
    def get_testsuites():
        return view.get_testsuites()

    return blueprint

class TestsuiteView:

    def __init__(self, logger : logging.Logger):
        self._logger : logging.Logger = logger.getChild(__name__)

    def get_testsuites(self):
        print('Test: Placeholder')
        return 'Test: Placeholder', 404
