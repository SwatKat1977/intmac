//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef MESSAGEDEFINITION_H
#define MESSAGEDEFINITION_H
#include <map>
#include <list>
#include <MsgProtocolDefs.h>
#include <messaging\iMessageDefinition.h>


namespace MessageProtocol
{

//  +----------------------------------------------------------------
//  Type Definitions
//  +----------------------------------------------------------------
typedef std::list<MessageID> MessagesList;
typedef std::map<MessageID, bool> MessagesMap;


class MessageDefinition : public iMessageDefinition
{
public:
    MessageDefinition( MessageID msgID
                     , const MessagesList reqs
                     , ClientServices::ClientService handlingService );

    ~MessageDefinition();

    MessageID GetID();

    ClientServices::ClientService GetHandlingService();

    bool MessageMeetsRequirements( MessageID msgID );

protected:
    MessageID m_id;
    MessagesMap m_requirements;
    ClientServices::ClientService m_handlingService;
};


}   //  namespace MessageProtocol


#endif   //  #define MESSAGEDEFINITION_H
