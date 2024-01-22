/*
-----------------------------------------------------------------------------
This source file is part of ITEMS
(Integrated Test Management Suite)
For the latest info, see https://github.com/SwatKat1977/intmac/

Copyright 2014-2024 Integrated Test Management Suite Development Team

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
-----------------------------------------------------------------------------
*/
#include "ServiceContext.h"
#include "apis/accountsSvc/AccountsSvcClient.h"
#include "SessionsManager.h"

namespace items { namespace gatewaySvc { namespace routes { namespace handshake {

    using namespace serviceFramework;

    class BasicAuthenticate : public ApiRoute
    {
    public:
        BasicAuthenticate (
            std::string name,
            std::shared_ptr< common::apis::accountsSvc::AccountsSvcClient> accountsSvcClient,
            std::shared_ptr < SessionsManager> sessionManager);

        ApiOutResponsePtr Route (const ApiIncomingReqPtr& request);

    private:
        std::shared_ptr< common::apis::accountsSvc::AccountsSvcClient> m_accountsSvcClient;
        std::shared_ptr < SessionsManager> m_sessionManager;
    };

    class Logout : public ApiRoute
    {
    public:
        Logout (
            std::string name,
            std::shared_ptr < SessionsManager> sessionsManager,
            serviceFramework::ConfigManager configManager);

        ApiOutResponsePtr Route (const ApiIncomingReqPtr& request);

    private:
        std::shared_ptr < SessionsManager> m_sessionsManager;
        serviceFramework::ConfigManager m_configManager;
    };

    class IsValidUserToken : public ApiRoute
    {
    public:
        IsValidUserToken (
            std::string name,
            std::shared_ptr < SessionsManager> sessionsManager,
            serviceFramework::ConfigManager configManager);

        ApiOutResponsePtr Route (const ApiIncomingReqPtr& request);

    private:
        std::shared_ptr < SessionsManager> m_sessionsManager;
        serviceFramework::ConfigManager m_configManager;
    };

} } } }   // namespace items::gatewaySvc::routes::handshake
