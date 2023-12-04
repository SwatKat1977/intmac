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
#include "ConfigManager.h"

namespace items
{
    namespace serviceFramework
    {
        class ServiceInitialiser
        {
        public:

            ServiceInitialiser (std::string name) : m_name(name)
            {}

            std::string& Name () { return m_name; }

            virtual bool CallInitialise () = 0;

        protected:
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

            void AddInitialiser (ServiceInitialiser *initialiser);

        private:
            ConfigManager m_configManager;
            std::string m_contextName;
            bool m_initialised;
            std::string m_iniFile;
            SectionsMap* m_initLayout;
            bool m_shutdownRequested;
            bool m_usingIniConfig;
            std::list<ServiceInitialiser*> m_initialisers;

/*
            virtual bool ServiceInitialise () { return true; }

            virtual void ServiceRun () { ; }

            virtual void ServiceStop () { ; }

*/

            bool InitialiseLogger ();

            bool ReadIniConfiguration ();
        };

    }   // namespace serviceFramework
}   // namespace items
