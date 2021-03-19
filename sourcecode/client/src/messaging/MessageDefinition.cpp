//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <messaging\MessageDefinition.h>


namespace MessageProtocol
{


MessageDefinition::MessageDefinition( MessageID msgID
                                    , const MessagesList reqs
                                    , ClientServices::ClientService handlingService )
    : m_id( msgID ), m_handlingService( handlingService )
{
    MessagesList::const_iterator it;

    for ( it = reqs.begin(); it != reqs.end(); it++ )
    {
        m_requirements[(*it)] = true;
    }
}


MessageDefinition::~MessageDefinition()
{
}


MessageID MessageDefinition::GetID()
{
    return m_id;
}


ClientServices::ClientService MessageDefinition::GetHandlingService()
{
    return m_handlingService;
}


bool MessageDefinition::MessageMeetsRequirements (MessageID msgID)
{
    bool okToProcess = false;

    //  If the 'Any' wildcard is detected then all message ID's are valid, this
    //  should be used with care to futureproof your code!
    if ( m_requirements.find( MsgFilter_Any ) != m_requirements.end() )
    {
        return true;
    }

    okToProcess = ( m_requirements.find(msgID ) == m_requirements.end() )
                ? false : true;

    return okToProcess;
}


}   //  namespace MessageProtocol
