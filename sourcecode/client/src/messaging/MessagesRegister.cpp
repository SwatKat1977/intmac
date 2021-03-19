//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <messaging\MessagesRegister.h>


namespace MessageProtocol
{


MessagesRegister::MessagesRegister()
{
}


MessagesRegister::~MessagesRegister ()
{
}


void MessagesRegister::RegisterMessage( iMessageDefinition *msgDef )
{
    m_msgDefinitions[msgDef->GetID()] = msgDef;
}


bool MessagesRegister::IsRegistered( MessageID msgID )
{
    bool isRegistered = false;

    if ( m_msgDefinitions.find( msgID ) != m_msgDefinitions.end() )
    {
        isRegistered = true;
    }
    
    return isRegistered;
}


iMessageDefinition *MessagesRegister::GetDefinition( MessageID msgID )
{
    if ( m_msgDefinitions.find( msgID ) != m_msgDefinitions.end() )
    {
        return m_msgDefinitions[msgID];
    }

    return NULL;
}


}   //  namespace MessageProtocol
