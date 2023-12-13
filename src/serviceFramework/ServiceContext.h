/*
-----------------------------------------------------------------------------
This source file is part of ITEMS
(Integrated Test Management Suite )
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
#include <string>
#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/protocol/http/outgoing/Response.hpp"
#include "ConfigManager.h"
#include "ApiRoute.h"

namespace items
{
    namespace serviceFramework
    {
        class ServiceContext;

        enum ServiceNetworkType
        {
            SERVICENETWORKTYPE_IPV4,
            SERVICENETWORKTYPE_IPV6
        };

        using ApiRoutePtr = std::shared_ptr<ApiRoute>;
        using ApiRoutesMap = std::map<std::string, ApiRoutePtr>;

        struct ServiceProviderEntry
        {
            std::string name;
            std::string address;
            v_uint16 networkPort;
            ServiceNetworkType networkType;
            std::shared_ptr<oatpp::network::tcp::server::ConnectionProvider> provider;
            std::shared_ptr<oatpp::web::server::HttpRouter> router;
            std::shared_ptr<oatpp::web::server::HttpConnectionHandler> connectionHandler;
        };

        using ProvidersMap = std::map<std::string, ServiceProviderEntry>;

        class ServiceModule
        {
        public:

            ServiceModule (std::string name) : m_name(name)
            {}

            void SetContext (ServiceContext *context) { m_context = context; }
            ServiceContext *GetContext () { return m_context; }

            std::string& Name () { return m_name; }

            virtual bool Initialise () { return true; }

            virtual void Execute () { }

            virtual void Shutdown () { }

        protected:
            ServiceContext *m_context;
            std::string m_name;
        };

        class ServiceContext
        {
        public:

            ServiceContext (std::string contextName);

            bool Initialise (SectionsMap* initLayout,
                             std::string iniFile = "");

            void Execute ();

            void NotifyShutdownRequested ();

            void AddServiceModule (ServiceModule *newModule);

            void AddServiceProvider (std::string name,
                                     std::string address,
                                     int networkPort,
                                     ServiceNetworkType networkType);

            void AddRoute (std::string providerName,
                           HTTPRequestMethod method,
                           std::string endpoint,
                           ApiRoute *route);

            ConfigManager& GetConfigManager () { return m_configManager; }

        private:
            ConfigManager m_configManager;
            std::string m_contextName;
            bool m_initialised;
            std::string m_iniFile;
            SectionsMap* m_initLayout;
            bool m_shutdownRequested;
            bool m_usingIniConfig;
            std::list<ServiceModule *> m_modules;

            ProvidersMap m_providers;
            std::list<std::shared_ptr<oatpp::network::Server>> m_servers;

            std::string HttpRequestMethodToStr (HTTPRequestMethod method);

            bool InitialiseLogger ();

            bool ReadIniConfiguration ();
        };

    }   // namespace serviceFramework
}   // namespace items
