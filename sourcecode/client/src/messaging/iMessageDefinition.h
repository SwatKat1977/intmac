//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IMESSAGEDEFINITION_H
#define IMESSAGEDEFINITION_H
#include <MsgProtocolDefs.h>
#include <clientServices\clientServicesDefs.h>


namespace MessageProtocol
{

const MessageID MsgFilter_Any = 0;


class iMessageDefinition
{
public:
    virtual ~iMessageDefinition() {}

    virtual MessageID GetID() = 0;

    virtual ClientServices::ClientService GetHandlingService() = 0;

    virtual bool MessageMeetsRequirements( MessageID msgID ) = 0;
};


}   //  namespace MessageProtocol

#endif   //  #define IMESSAGEDEFINITION_H
