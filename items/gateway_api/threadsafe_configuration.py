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
from configuration.configuration_manager import ConfigurationManager
from thread_safe_singleton import ThreadSafeSingleton

class ThreadafeConfiguration(ConfigurationManager, metaclass = ThreadSafeSingleton):
    """ Thread-safe singleton for the config """

    @property
    def logging_log_level(self) -> str:
        """ Configuration property : Logging | log level """
        return ThreadafeConfiguration().get_entry('loggings', 'log_level')

    @property
    def sessions_redis_host(self) -> str:
        """ Configuration property : Sessions REDIS | Hostname or IP address """
        return ThreadafeConfiguration().get_entry('sessions_redis', 'host')

    @property
    def sessions_redis_port(self) -> str:
        """ Configuration property : Sessions REDIS | Port """
        return ThreadafeConfiguration().get_entry('sessions_redis', 'port')

    @property
    def sessions_redis_retries(self) -> str:
        """ Configuration property : Sessions REDIS | Max Retries """
        return ThreadafeConfiguration().get_entry('sessions_redis', 'retries')

    @property
    def internal_apis_accounts_svc(self) -> str:
        """ Configuration property : Internal APIs | Accounts Service base path """
        return ThreadafeConfiguration().get_entry(
            'internal_apis', 'accounts_svc')

    @property
    def internal_apis_cms_svc(self) -> str:
        """ Configuration property : Internal APIs | CMS Service base path """
        return ThreadafeConfiguration().get_entry(
            'internal_apis', 'cms_svc')
