'''
Copyright 2023 Integrated Test Management Suite

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
    SECTION_LOGGING : str = 'logging'
    SECTION_BACKEND : str = 'backend'

    ITEM_LOGGING_LOG_LEVEL : str = 'log_level'
    LOG_LEVEL_DEBUG : str = 'DEBUG'
    LOG_LEVEL_INFO : str = 'INFO'

    ITEM_BACKEND_ENGINE : str = 'engine'
    ENGINE_INTERNAL : str = 'internalDB'

    ITEM_BACKEND_INTERNAL_DB_FILENAME : str = 'internal_db_filename'
    ITEM_BACKEND_CREATE_DB_IF_MISSING = "create_db_if_missing"

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
        ConfigurationConstants.SECTION_BACKEND: [
            configuration_setup.ConfigurationSetupItem(
                ConfigurationConstants.ITEM_BACKEND_ENGINE,
                configuration_setup.ConfigItemDataType.STRING,
                valid_values=[ConfigurationConstants.ENGINE_INTERNAL],
                              default_value=ConfigurationConstants.ENGINE_INTERNAL),

            configuration_setup.ConfigurationSetupItem(
                ConfigurationConstants.ITEM_BACKEND_INTERNAL_DB_FILENAME,
                configuration_setup.ConfigItemDataType.STRING,
                default_value="items_accounts_svc.db"),

            configuration_setup.ConfigurationSetupItem(
                ConfigurationConstants.ITEM_BACKEND_CREATE_DB_IF_MISSING,
                configuration_setup.ConfigItemDataType.STRING,
                valid_values=['YES', 'NO'], default_value="NO")
        ]
    }
)
