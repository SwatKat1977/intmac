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
import json
import logging
from time import sleep, time
from uuid import uuid4
import redis
from logging_consts import LOGGING_DATETIME_FORMAT_STRING, \
                           LOGGING_DEFAULT_LOG_LEVEL, \
                           LOGGING_LOG_FORMAT_STRING

class RedisInterface:
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
        status = False

        try:
            self._redis = redis.Redis(host = self._host, port = self._port)
            self._redis.get('invalid_key')
            status = True

        except redis.exceptions.ConnectionError as ex:
            self._logger.critical('Unable to commect to redis, reason: %s', ex)

        return status

    def test1(self):
        trial = {"str": "example", "int": 2001}
        self._redis.set('jsondata', '{"str": "example", "int": 20}' )
        self._redis.set('jsondata', json.dumps(trial) )

    def test2(self):
        t = self._redis.get('jsondata')
        t = json.loads(t)
        print(t)

    def acquire_lock(self, lockname, timeout = 2):

        lock_name = f'lock_{lockname}'
        identifier = str(uuid4())
        acquired_lock = None

        end = time() + timeout

        while time() < end and not acquired_lock:
            if self._redis.setnx(lock_name, identifier):
                acquired_lock = identifier

            sleep(0.001)

        self._redis.delete(lock_name)

        return acquired_lock

r = RedisInterface('localhost', '6379')
r.initialise()
r.test1()
r.test2()
yu = r.acquire_lock('trial')
print('lock id:', yu)
