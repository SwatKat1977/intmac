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
from abc import ABCMeta, abstractmethod
from intmac_exception import IntmacException

class Application(metaclass=ABCMeta):
    ''' Application framework class. '''

    @property
    def is_running(self) -> bool:
        '''
        Getter for is_running property.

        returns:
            Is running flag (boolean).
        '''
        return self._is_running

    @is_running.setter
    def is_running(self, is_running : bool) -> None:
        '''
        Setter for is_running property.

        parameters:
            is_running New value for property.
        '''
        self._is_running = is_running

    def __init__(self):
        self._is_initialised = False
        self._is_running = False

    async def run(self) -> None:
        '''
        Start the main application, this is a looping method.
        '''

        if not self._is_initialised:
            raise IntmacException('Application not initialised')

        # Run the main loop.
        while self._is_running:
            self._main_loop()

        # Perform any shutdown required once the application has ended.
        self._shutdown()

    def initialise(self) -> bool:
        '''
        Application initialisation.  It should return a boolean
        (True => Successful, False => Unsuccessful), upon success
        self._is_initialised is set to True.

        returns:
            Boolean: True => Successful, False => Unsuccessful.
        '''
        if self._initialise() is True:
            self._is_initialised = True
            init_status = True

        else:
            init_status = False

        return init_status

    @abstractmethod
    def _initialise(self) -> bool:
        '''
        Abstract method for the application initialisation.  It should return
        a boolean (True => Successful, False => Unsuccessful).

        returns:
            Boolean: True => Successful, False => Unsuccessful.
        '''
        return True

    @abstractmethod
    def _main_loop(self) -> None:
        ''' Abstract method for main application. '''

    @abstractmethod
    def _shutdown(self):
        ''' Abstract method for application shutdown. '''
