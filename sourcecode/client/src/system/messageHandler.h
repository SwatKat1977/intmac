//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <vector>
#include <System/iMessageHandler.h>
#include <System/iCommunicationsLayer.h>
#include <messaging/iMessagesRegister.h>
#include <clientServices\ClientServicesManager.h>


namespace Comms
{


struct PartialIncomingMsg
{
    int m_amountRead;
    char m_messageHeader[MessageProtocol::MessageHeaderSize];
    char m_messageBody[MessageProtocol::MaxMessageBodySize];
};

struct PartialOutgoingMsg
{
    int m_totalMessageSize;
    int m_currentMessageSize;
    MessageProtocol::MessageHeader m_messageHeader;
    char m_messageBody[MessageProtocol::MaxMessageBodySize];
};


class MessageHandler : public iMessageHandler
{
public:
    MessageHandler( iCommunicationsLayer *commsLayer
                  , MessageProtocol::iMessagesRegister *messagesRegister
                  , ClientServices::ClientServicesManager *clientServicesManager );

    ~MessageHandler();

    void ReceiveOutgoingMessage( iCommsMessage *msg );

    void ProcessIncomingMessages();

    void ProcessOutgoingMessages();
 
    MessageProtocol::MessageID GetCurrentMessage() { return m_currentMessageID; }

private:
    iCommunicationsLayer *m_commsLayer;
    std::vector <iCommsMessage> m_outgoingMsgs;
    bool m_hasPartialIncomingMsg;
    bool m_hasPartialOutgoingMsg;
    PartialIncomingMsg *m_partialIncomingMsg;
    PartialOutgoingMsg *m_partialOutgoingMsg;
    MessageProtocol::iMessagesRegister *m_messagesRegister;
    MessageProtocol::MessageID m_currentMessageID;
    ClientServices::ClientServicesManager *m_clientServicesManager;

    bool RetrieveMessageHeader( MessageProtocol::MessageHeader *header );

    bool RetrieveMessageBody( MessageProtocol::MessageHeader *header
                            , char *body );
};

}

#endif
