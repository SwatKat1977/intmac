//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <system/MessageHandler.h>


namespace Comms
{


MessageHandler::MessageHandler( iCommunicationsLayer *commsLayer
                              , MessageProtocol::iMessagesRegister *messagesRegister
                              , ClientServices::ClientServicesManager *clientServicesManager )

              : m_commsLayer( commsLayer )
              , m_messagesRegister( messagesRegister )
              , m_hasPartialIncomingMsg( false )
              , m_hasPartialOutgoingMsg( false )
              , m_clientServicesManager( clientServicesManager )
              , m_currentMessageID( MessageProtocol::MsgFilter_Any )
{
    m_partialIncomingMsg = new PartialIncomingMsg;
    m_partialOutgoingMsg = new PartialOutgoingMsg;

    memset( m_partialIncomingMsg, 0, sizeof( PartialIncomingMsg ) );
    memset( m_partialOutgoingMsg, 0, sizeof( PartialOutgoingMsg ) );
}


MessageHandler::~MessageHandler()
{
    if ( m_partialIncomingMsg ) delete m_partialIncomingMsg;
    if (m_partialOutgoingMsg ) delete m_partialOutgoingMsg;
}


void MessageHandler::ReceiveOutgoingMessage( iCommsMessage *msg )
{
}


void MessageHandler::ProcessIncomingMessages()
{
    MessageProtocol::MessageHeader header;
    char body[MessageProtocol::MaxMessageBodySize] = "\0";
    MessageProtocol::iMessageDefinition *msgDefinition;

    if ( ( m_commsLayer->IsConnected() == false ) || 
         ( m_commsLayer->IsIncomingDataWaiting() == false ) ) return;

    if ( RetrieveMessageHeader( &header ) == false ) return;

    if ( header.m_bodyLength > 0 )
    {
        if ( RetrieveMessageBody( &header, body ) == false ) return;
    }

    if ( m_messagesRegister->IsRegistered( header.m_ID ) == false )
    {
        std::cout << "[DEBUG] Message ID of '" << header.m_ID << "' isn't valid!" << std::endl;
        return;
    }
    
    bool msgHandleSuccessState = false;

    //  Get message definition and check previous message meets requirements
    msgDefinition = m_messagesRegister->GetDefinition( header.m_ID );
    if ( msgDefinition->MessageMeetsRequirements( m_currentMessageID ) == false )
    {
        std::cout << "[DEBUG] Message " << header.m_ID << " is out of order!" << std::endl;
        return;
    }

    switch( msgDefinition->GetHandlingService() )
    {
    case ClientServices::ClientService_sessionsManager:
        msgHandleSuccessState = m_clientServicesManager->GetSessionManagerSrv()->HandleMessage( &header, body );
        break;

    default:
        break;
    }

    if ( msgHandleSuccessState )
    {
        m_currentMessageID = header.m_ID;
    }
}


void MessageHandler::ProcessOutgoingMessages()
{
#ifdef WORK_IN_PROGRESS
    if ( outgoinglist is greater then 0 )
    {
        m_commsLayer->Write( data, size, offset );
    }
#endif
}
 

bool MessageHandler::RetrieveMessageHeader( MessageProtocol::MessageHeader *header )
{
    int currentMsgSize = 0;
    int headerAmountToRead = 0;
    char readBuffer[MessageProtocol::MessageHeaderSize] = "\0";
    int bytesRead = 0;

    currentMsgSize = ( m_hasPartialIncomingMsg == true ) 
                     ? m_partialIncomingMsg->m_amountRead : 0;
    headerAmountToRead = MessageProtocol::MessageHeaderSize - currentMsgSize;
 
    if ( currentMsgSize >= MessageProtocol::MessageHeaderSize )
    {
        memcpy( readBuffer
              , m_partialIncomingMsg->m_messageHeader 
              , MessageProtocol::MessageHeaderSize );
        bytesRead = MessageProtocol::MessageHeaderSize;
    }
    else
    {
        if ( m_hasPartialIncomingMsg )
        {
            memcpy( readBuffer
                  , m_partialIncomingMsg->m_messageHeader
                  , m_partialIncomingMsg->m_amountRead );
        }

        int currBytesRead = m_commsLayer->Read( headerAmountToRead
                                              , readBuffer + currentMsgSize );
        bytesRead += currBytesRead;
    }

    if ( bytesRead == 0 ) return false;

    if ( bytesRead != MessageProtocol::MessageHeaderSize  )
    {
        m_hasPartialIncomingMsg = true;
        m_partialIncomingMsg->m_amountRead = bytesRead;

        memcpy( m_partialIncomingMsg->m_messageHeader 
              , readBuffer
              , bytesRead );
        return false;
    }

    memcpy( header, readBuffer, MessageProtocol::MessageHeaderSize );

    return true;
}


bool MessageHandler::RetrieveMessageBody( MessageProtocol::MessageHeader *header
                                        , char *body )
{
    int amountToRead = 0;
    char readBuffer[MessageProtocol::MaxMessageBodySize] = "\0";
    int partialMsgSize = 0;
    int offset = 0;
    int readBytes = 0;

    partialMsgSize = ( m_hasPartialIncomingMsg == true )  
                     ? m_partialIncomingMsg->m_amountRead : 0;

    amountToRead = header->m_bodyLength;

    if ( partialMsgSize > MessageProtocol::MessageHeaderSize )
    {
        amountToRead = partialMsgSize - MessageProtocol::MessageHeaderSize;

        offset = header->m_bodyLength 
               - ( partialMsgSize - MessageProtocol::MessageHeaderSize );

        memcpy( readBuffer
              , m_partialIncomingMsg->m_messageBody
              , offset );
    }

    readBytes = m_commsLayer->Read( amountToRead, readBuffer + offset );

    if ( readBytes == 0 ) return false;

    readBytes += offset;

    if ( readBytes != header->m_bodyLength )
    {
        m_hasPartialIncomingMsg = true;
        m_partialIncomingMsg->m_amountRead = MessageProtocol::MessageHeaderSize
                                           + readBytes;

        memcpy( m_partialIncomingMsg->m_messageBody
              , readBuffer
              , readBytes );
        return false;
    }

    memcpy( body, readBuffer, header->m_bodyLength );

    return true;
}

}
