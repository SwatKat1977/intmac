//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef ICOMMSMESSAGE_H
#define ICOMMSMESSAGE_H


#include <MsgProtocolDefs.h>

namespace Comms
{


class iCommsMessage
{
public:
    virtual ~iCommsMessage() {}

    virtual MessageProtocol::MessageID GetMessageID() = 0;

    virtual unsigned int GetBodyLength() = 0;

    virtual char *GetBody() = 0;
};

}

#endif
