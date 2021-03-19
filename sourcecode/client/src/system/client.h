//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef CLIENT_H
#define CLIENT_H

#include <wx/app.h>
#include <wx/wxprec.h>
#include <system/commsThread.h>
#include <system/iCommunicationsLayer.h>
#include <system/MessageHandler.h>
#include <dataModels/projectModel.h>
#include <iDOMParser.h>
#include <platformUtils.h>
#include <system/eventHandler.h>
#include <messaging\messagesRegister.h>
#include <clientServices\ClientServicesManager.h>
#include <clientServices\SessionManagerClientSrv.h>


class Client : public wxApp
{
public:
    bool OnInit( );

    int OnExit();	

private:
    CommsThread *m_commsThread;
    StateModel *m_stateModel;
    iCommunicationsLayer *m_commsLayer;
    Comms::MessageHandler *m_msgHandler;
    ProjectModel *m_projectModel;
    iDOMParser *m_DOMParser;
    PlatformUtils *m_XMLPlatformUtils;
    EventHandler *m_eventHandler;
    MessageProtocol::MessagesRegister *m_messagesRegister;
    ClientServices::ClientServicesManager *m_clientServicesManager;
    bool InitialiseXMLParser();

    void InitialiseMessagingSystem();
};

#endif
