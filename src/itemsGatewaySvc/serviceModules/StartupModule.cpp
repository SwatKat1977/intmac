/*
-----------------------------------------------------------------------------
This source file is part of ITEMS
(Integrated Test Management Suite)
For the latest info, see https://github.com/SwatKat1977/intmac/

Copyright 2014-2023 Integrated Test Management Suite Development Team

    This program is free software : you can redistribute it and /or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see < https://www.gnu.org/licenses/>.

The following is based on Ogre3D code:
* GetEnv from os-int.h
-----------------------------------------------------------------------------
*/
#include <filesystem>
#include "spdlog/spdlog.h"
#include "Definitions.h"
#include "SessionsManager.h"
#include "StartupModule.h"
#include "Logger.h"
#include "Version.h"

namespace items { namespace gatewaySvc {

    using namespace serviceFramework;

    StartupModule::StartupModule (std::string name)
        : ServiceModule (name)
    {
    }

    bool StartupModule::Initialise ()
    {
        LOGGER->info ("ITEMS Gateway Microservice V{0}.{1}.{2}-{3}",
            common::VERSION_MAJOR, common::VERSION_MINOR,
            common::VERSION_PATCH, common::VERSION_TAG);
        LOGGER->info (common::COPYRIGHT_TEXT);
        LOGGER->info (common::LICENSE_TEXT);

        LOGGER->info ("|=====================|");
        LOGGER->info ("|=== Configuration ===|");
        LOGGER->info ("|=====================|");

        LOGGER->info ("[LOGGING]");
        LOGGER->info ("-> Log Level      : {0}",
            m_context->GetConfigManager ().GetStringEntry (
                "logging", "log_level").c_str ());
        LOGGER->info ("-> Log To Console : {0}",
            m_context->GetConfigManager ().GetStringEntry (
                "logging", "log_to_console").c_str ());
        LOGGER->info ("-> Log Filename   : {0}",
            m_context->GetConfigManager ().GetStringEntry (
                "logging", "log_filename").c_str ());
        LOGGER->info ("-> Max File Size  : {0:d}",
            m_context->GetConfigManager ().GetIntEntry (
                "logging", "max_file_size"));
        LOGGER->info ("-> Max File Count : {0:d}",
            m_context->GetConfigManager ().GetIntEntry (
                "logging", "max_file_count"));
        LOGGER->info ("-> Log Format     : {0}",
            m_context->GetConfigManager ().GetStringEntry (
                "logging", "log_format").c_str ());

        LOGGER->info ("[SESSIONS]");
        LOGGER->info ("-> Session TYmeout (secs) : {0:d}",
            m_context->GetConfigManager ().GetIntEntry (
                "sessions", "timeout_secs"));

        LOGGER->info ("[INTERNAL APIS]");
        LOGGER->info ("-> Accounts Service API : {0}",
            m_context->GetConfigManager ().GetStringEntry (
                "internal_apis", "accounts_svc").c_str ());
        LOGGER->info ("-> CMS Service API      : {0}",
            m_context->GetConfigManager ().GetStringEntry (
                "internal_apis", "cms_svc").c_str ());

        if (!AddServiceProviders ())
        {
            return false;
        }

        return true;
    }

    bool StartupModule::AddServiceProviders ()
    {
        try
        {
            m_context->AddServiceProvider (
                SERVICE_PROVIDER_API_NAME,
                "0.0.0.0",
                SERVICE_PROVIDER_API_PORT,
                SERVICENETWORKTYPE_IPV4);
        }
        catch (std::runtime_error &e)
        {
            LOGGER->critical ("Unable to create service provider '{0}' : {1}",
                SERVICE_PROVIDER_API_NAME, e.what ());
            return false;
        }

        return true;
    }

} }   // namespace items::gatewaySvc
