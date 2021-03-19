//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef MESSAGESREGISTER_H
#define MESSAGESREGISTER_H
#include <map>
#include <messaging\iMessagesRegister.h>


namespace MessageProtocol
{

typedef std::map<MessageID, iMessageDefinition *> MessageDefinitionMap;


class MessagesRegister : public iMessagesRegister
{
public:
    MessagesRegister();

    ~MessagesRegister();

    void RegisterMessage( iMessageDefinition *msgDef );

    bool IsRegistered( MessageID msgID );

    iMessageDefinition *GetDefinition( MessageID msgID );

protected:
    MessageDefinitionMap m_msgDefinitions;
};


}   //  namespace MessageProtocol


#endif   //  #define MESSAGESREGISTER_H
