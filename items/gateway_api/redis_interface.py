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
import json
import logging
from time import sleep, time
from typing import Union
from uuid import uuid4
import redis
from items_exception import ItemsException
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING
from logon_type import LogonType

class RedisInterface:
    """ REDIS interface wrapper class for ITEMS project """
    __slots__ = ["_host", "_logger", "_port", "_redis"]

    def __init__(self, host : str, port : int) -> None:
        self._host = host
        self._port = port
        self._redis = None

        self._logger = logging.getLogger(__name__)
        log_format= logging.Formatter(LOGGING_LOG_FORMAT_STRING,
                                      LOGGING_DATETIME_FORMAT_STRING)
        console_stream = logging.StreamHandler()
        console_stream.setFormatter(log_format)
        self._logger.setLevel(LOGGING_DEFAULT_LOG_LEVEL)
        self._logger.addHandler(console_stream)

    def initialise(self) -> bool:
        """
        Initialise the REDIS interface, it returns a boolean status.

        returns:
            bool - Initialisation status boolean
        """

        status = False

        try:
            self._redis = redis.Redis(host = self._host, port = self._port)
            self._redis.get('invalid_key')
            status = True

        except redis.exceptions.ConnectionError as ex:
            self._logger.critical('Unable to commect to redis, reason: %s', ex)

        return status

    def add_auth_session(self, email_address : str, token : str,
                               logon_type : LogonType) -> bool:
        """
        Add an authentication session to the REDIS database. It will attempt to
        lock the record before adding it to ensure concurrency consistency.

        parameters:
            email_address - Email address of the user\n
            token - Unique token specific for the session\n
            logon_type - Type of logon that occurred - currently only BASIC

        returns:
            bool - Success status of the add.
        """

        status = False

        # Session timeout isn't implemented so the expiry will always be set to
        # a value of 0 (no expiry)

        session_data = {
            "email" : email_address,
            "logon type" : logon_type.value,
            "session expiry" : 0,
            "token" : token
        }

        lock_id = self._acquire_lock(email_address)
        if not lock_id:
            self._logger.error("Unable to get a REDIS lock for %s",
                               email_address)

        else:
            # If you logon a second time it will just invalid the previous one
            # currently. This logic should be improved at a later date!!
            self._redis.set(email_address, json.dumps(session_data))
            self._release_lock(email_address, lock_id)
            status = True

        return status

    def del_auth_session(self, email_address : str) -> bool:
        """
        Remove an authentication session. It will attempt to lock the record
        before deleting it to ensure concurrency consistency.

        parameters:
            email_address - Email address of the user

        returns:
            bool - Success status of the del.
        """

        status = False

        lock_id = self._acquire_lock(email_address)
        if not lock_id:
            self._logger.error("Unable to get a REDIS lock for %s",
                               email_address)

        else:
            # If you logon a second time it will just invalid the previous one
            # currently. This logic should be improved at a later date!!
            self._redis.delete(email_address)
            self._release_lock(email_address, lock_id)
            status = True

        return status

    def is_valid_session(self, email_address : str, token : str) -> bool:
        """
        Verify if a session token for a given is valid.

        parameters:
            email_address - Email address of the user\n
            token - Token value

        returns:
            bool - Validity boolean.
        """

        status = False

        session = self._redis.get(email_address)

        if session:
            json_data = json.loads(session)
            recv_token = json_data["token"]
            status = recv_token == token

        return status

    def _acquire_lock(self, name : str, timeout : int = 2) -> Union[None, bytes]:
        """
        Acquire a lock, retrying for timeout number of seconds.

        parameters:
            name - Name of lock\n
            timeout - Amount of time in seconds to try acquiring the lock

        returns:
          str | None - The unique lock identifier string is returned if the
                       call was successful, otherwise None is returned.
        """

        lock_name = f'lock_{name}'
        identifier = str(uuid4()).encode("UTF-8")
        acquired_lock = None

        end = time() + timeout

        while time() < end and not acquired_lock:
            if self._redis.setnx(lock_name, identifier):
                acquired_lock = identifier

            sleep(0.001)

        return acquired_lock

    def _release_lock(self, lock : str, identifier : str) -> bool:
        """
        Attempt to release a lock. If the lock name doesn't exist or the
        identifier doesn't match then an exception is raised.

        parameters:
            lock - Name of the lock (usually record key)\n
            identifier - Lock identifier UUID

        returns:
            bool - Status of lock release attempt, True is success.
        """

        lock_name = f'lock_{lock}'
        status = False

        lock = self._redis.get(lock_name)

        if lock and lock == identifier:
            self._redis.delete(lock_name)
            status = True

        else:
            self._logger.error(("Failed attempt releasing REDIS lock '%s' "
                                "with id '%s'"), lock_name, identifier)
            raise ItemsException('Invalid REDIS lock')

        return status
