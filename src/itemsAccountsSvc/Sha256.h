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
-----------------------------------------------------------------------------
*/
#ifndef SHA256_H
#define SHA256_H
#include <string>

namespace items
{
    namespace accountsSvc
    {
        class SHA256
        {
        protected:
            typedef unsigned char uint8;
            typedef unsigned int uint32;
            typedef unsigned long long uint64;

            const static uint32 sha256_k[];
            static const unsigned int SHA224_256_BLOCK_SIZE = (512 / 8);
        public:
            void init ();
            void update (const unsigned char* message, unsigned int len);
            void final (unsigned char* digest);
            static const unsigned int DIGEST_SIZE = (256 / 8);

        protected:
            void transform (const unsigned char* message, unsigned int block_nb);
            unsigned int m_tot_len;
            unsigned int m_len;
            unsigned char m_block[2 * SHA224_256_BLOCK_SIZE];
            uint32 m_h[8];
        };

        std::string GenerateSha256 (std::string input);

    }   // namespace accountsSvc
}   // namespace items

#endif
