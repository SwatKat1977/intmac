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
from configuration import configuration_setup

class ConfigurationConstants:
    ''' Constants for the microservice configuration. '''

    SECTION_LOGGING : str = 'logging'
    SECTION_INTERNAL_APIS : str = 'internal_apis'

    ITEM_LOGGING_LOG_LEVEL : str = 'log_level'
    LOG_LEVEL_DEBUG : str = 'DEBUG'
    LOG_LEVEL_INFO : str = 'INFO'

    ITEM_INTERNAL_APIS_GATEWAY : str = 'gateway_svc'

CONFIGURATION_LAYOUT = configuration_setup.ConfigurationSetup(
    {
        ConfigurationConstants.SECTION_LOGGING: [
            configuration_setup.ConfigurationSetupItem(
                ConfigurationConstants.ITEM_LOGGING_LOG_LEVEL,
                configuration_setup.ConfigItemDataType.STRING,
                valid_values=[ConfigurationConstants.LOG_LEVEL_DEBUG,
                              ConfigurationConstants.LOG_LEVEL_INFO],
                              default_value=ConfigurationConstants.LOG_LEVEL_INFO)
        ],
        ConfigurationConstants.SECTION_INTERNAL_APIS: [
            configuration_setup.ConfigurationSetupItem(
                "gateway_svc", configuration_setup.ConfigItemDataType.STRING,
                is_required=True),
        ]
    }
)