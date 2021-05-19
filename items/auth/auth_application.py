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
from application import Application
from views.placeholder_view import create_placeholder_blueprint

class AuthApplication(Application):

    def __init__(self, quart_instance):
        super().__init__()
        self._quart_instance = quart_instance

    def _initialise(self) -> bool:

        placeholder_blueprint = create_placeholder_blueprint()
        self._quart_instance.register_blueprint(placeholder_blueprint)

        return True

    def _main_loop(self) -> None:
        ''' Abstract method for main application. '''

    def _shutdown(self):
        ''' Abstract method for application shutdown. '''
