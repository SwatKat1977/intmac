//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <system/commsThread.h>


CommsThread::CommsThread( iCommunicationsLayer *commsLayer
                        , Comms::MessageHandler *msgHandler )
            : m_commsLayer( commsLayer )
            , m_msgHandler( msgHandler )
{
}


CommsThread::~CommsThread()
{
}


wxThread::ExitCode CommsThread::Entry()
{
    while ( !TestDestroy() )
    {
        m_commsLayer->ManageConnectionToServer();

        if ( m_commsLayer->IsConnected() == true )
        {
            m_msgHandler->ProcessIncomingMessages();
            m_msgHandler->ProcessOutgoingMessages();
        }
    }

    //  Handle thread shutdown

    return NULL;
}