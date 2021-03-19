//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IMESSAGESREGISTER_H
#define IMESSAGESREGISTER_H
#include <MsgProtocolDefs.h>
#include <messaging\iMessageDefinition.h>


namespace MessageProtocol
{


class iMessagesRegister
{
public:
    virtual ~iMessagesRegister() {}

    virtual void RegisterMessage( iMessageDefinition *msgDef ) = 0;

    virtual bool IsRegistered( MessageID msgID ) = 0;

    virtual iMessageDefinition *GetDefinition( MessageID msgID ) = 0;
};


}   //  namespace MessageProtocol


#endif   //  #define IMESSAGESREGISTER_H
