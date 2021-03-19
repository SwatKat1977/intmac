//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef MSGPROTOCOLDEFS_H
#define MSGPROTOCOLDEFS_H


namespace MessageProtocol
{

//  +----------------------------------------------------------------
//  Type Definitions
//  +----------------------------------------------------------------
typedef long MessageID;


//  +----------------------------------------------------------------
//  Common Structures
//  +----------------------------------------------------------------
struct MessageHeader
{
    MessageID m_ID;
    unsigned int m_bodyLength;
};


//  +----------------------------------------------------------------
//  Constants
//  +----------------------------------------------------------------
const int MessageHeaderSize = sizeof( MessageHeader );
const int MaxMessageBodySize = 512;


}   //  namespace MessageProtocol

#endif   //  #define MSGPROTOCOLDEFS_H
