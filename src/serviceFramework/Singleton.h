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
#ifndef SINGLETON_H
#define SINGLETON_H
#include "Platform.h"

namespace items
{
    namespace serviceFramework
    {

        template <typename T> class Singleton
        {
        public:

            Singleton ()
            {
                ItemsAssert (!m_singletonInstance,
                    "Multiple singleton instances not permitted");
                m_singletonInstance = static_cast<T*>(this);
            }

            ~Singleton ()
            {
                ItemsAssert (m_singletonInstance,
                    "Singleton instances doesn't exist during destruction");
                m_singletonInstance = nullptr;
            }

            static T& GetSingleton ()
            {
                ItemsAssert (m_singletonInstance,
                    "Singleton instances doesn't exist");
                return *m_singletonInstance;
            }

        protected:
            static T* m_singletonInstance;

        private:
            // The explicit private copy constructor is forbidden.
            Singleton (const Singleton<T>&);

            // The private operator '=' is forbidden.
            Singleton& operator= (const Singleton<T>&) { }
        };

    }   // namespace serviceFramework
}   // namespace items

#endif
