//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef CLIENTSERVICESDEFS_H
#define CLIENTSERVICESDEFS_H
#include <MsgProtocolDefs.h>


namespace ClientServices
{


enum ClientService
{
    ClientService_sessionsManager
};


//  +----------------------------------------------------------------
//  Constants
//  +----------------------------------------------------------------
const MessageProtocol::MessageID MsgFilter_Any = 0;


}   //  namespace ClientServices

#endif   //  #define CLIENTSERVICESDEFS_H
