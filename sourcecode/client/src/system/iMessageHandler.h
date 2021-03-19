//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IMESSAGEHANDLER_H
#define IMESSAGEHANDLER_H

#include<system/iCommsMessage.h>

namespace Comms
{


class iMessageHandler
{
public:
    virtual ~iMessageHandler() {}

    virtual void ReceiveOutgoingMessage( iCommsMessage *msg ) = 0;

    virtual void ProcessIncomingMessages() = 0;

    virtual void ProcessOutgoingMessages() = 0;
};

}

#endif
