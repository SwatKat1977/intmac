//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <system/client.h>
#include <messaging\MessageDefinitions.h>
#include <hci/mainDialog/mainDialog.h>
#include <system/shMemConnectionInfo.h>
#include <system/communicationsLayerFactory.h>
#include <DOMParserFactory.h>
#include <XMLParseException.h>


std::string shMemReadBufferName = "Global\\intMacReadBuffer";
std::string shMemWriteBufferName = "Global\\intMacWriteBuffer";
int shMemReadWriteBufferSize = 1024;


bool Client::OnInit( )
{
    MainDialog *mainDialog = NULL;
    CommunicationsLayerFactory *commsLayerFactory = NULL;

    if ( !wxApp::OnInit( ) )
    {
        return false;
    }
    
    if ( InitialiseXMLParser() == false )
    {
        wxMessageBox( "Unable to initialise XML Parser system..."
                    , "FATAL Error"
                    , wxOK | wxICON_ERROR );
        OnExit();
        return false;
    }

    m_commsThread = nullptr;

    commsLayerFactory = new CommunicationsLayerFactory;
    m_commsLayer = commsLayerFactory->Create( CommsLayerType_sharedMemory );
    delete commsLayerFactory;

    InitialiseMessagingSystem();

    //  Create data models
    m_stateModel = new StateModel;
    m_projectModel = new ProjectModel;

    m_clientServicesManager = new ClientServices::ClientServicesManager( m_stateModel
                                                                       , m_projectModel
                                                                       , m_DOMParser );

    m_msgHandler = new  Comms::MessageHandler( m_commsLayer
                                             , m_messagesRegister
                                             , m_clientServicesManager );

    m_stateModel->SetMessageHandler( m_msgHandler );
    m_projectModel->SetMessageHandler( m_msgHandler );

    m_eventHandler = new EventHandler;

    m_commsThread = new CommsThread( m_commsLayer
                                   , m_msgHandler );

    ShMemConnectionInfo connInfo( shMemReadBufferName
                                , shMemReadWriteBufferSize
                                , shMemWriteBufferName
                                , shMemReadWriteBufferSize );

    //  Temporary
    if ( m_commsLayer->ConnectToServer( &connInfo ) == false )
    {
        wxMessageBox( "Unable to connect to server..."
                    , "Connection Error"
                    , wxOK | wxICON_ERROR );
        OnExit();
        return false;
    }

    if ( m_commsThread->Run() != wxTHREAD_NO_ERROR )
    {
        wxMessageBox( "Unable start comms thread, aborting..."
                    , "FATAL Error"
                    , wxOK | wxICON_ERROR );
        delete m_commsThread;
        m_commsThread = NULL;
        OnExit();
    }

    bool hiddenAuthLoginOK = false;

    while ( hiddenAuthLoginOK == false )
    {
        if ( m_stateModel->GetProjectsList().m_listLoaded == true )
        {
            hiddenAuthLoginOK = true;
        }

        Sleep( 10 );
    }


    MessageProtocol::MessageID currentMsgID = m_msgHandler->GetCurrentMessage();

    while ( currentMsgID < MessageProtocol::MsgID_ProjectsListBulkData )
    {
        Sleep( 10 );
        currentMsgID = m_msgHandler->GetCurrentMessage();
    }    

    mainDialog = new MainDialog( m_projectModel
                               , m_stateModel
                               , m_eventHandler );
    
    mainDialog->Show( true );

    return true;
}


void Client::InitialiseMessagingSystem()
{
    m_messagesRegister = new MessageProtocol::MessagesRegister;

    m_messagesRegister->RegisterMessage( &MessageProtocol::MsgDef_HandshakeResponse );
    m_messagesRegister->RegisterMessage( &MessageProtocol::MsgDef_AuthenticationStatus );
    m_messagesRegister->RegisterMessage( &MessageProtocol::MsgDef_BeginProjectsBulkImport );
    m_messagesRegister->RegisterMessage (&MessageProtocol::MsgDef_ProjectsListBulkData );
    m_messagesRegister->RegisterMessage (&MessageProtocol::MsgDef_ProjectSelectionStatus );
    m_messagesRegister->RegisterMessage( &MessageProtocol::MsgDef_BeginProjectAssetsBulkData );
    m_messagesRegister->RegisterMessage( &MessageProtocol::MsgDef_ProjectAssetsBulkData );
}


int Client::OnExit()
{
    if ( m_commsThread != nullptr )
    {
        if ( m_commsThread->IsRunning() )
        {
            if ( m_commsThread->Delete() != wxTHREAD_NO_ERROR )
            {
                wxLogError( "Can't delete the thread!" );
            }
        }
        else
        {
            delete m_commsThread;
        }

    }

    if ( m_stateModel ) delete m_stateModel;
    if ( m_commsLayer ) delete m_commsLayer;
    if ( m_msgHandler ) delete m_msgHandler;
    if ( m_projectModel ) delete m_projectModel;
    if ( m_DOMParser ) delete m_DOMParser;

    if ( m_XMLPlatformUtils )
    {
        m_XMLPlatformUtils->Terminate();
        delete m_XMLPlatformUtils;
    }

    if ( m_eventHandler ) delete m_eventHandler;

    return 0;
}


bool Client::InitialiseXMLParser()
{
    DOMParserFactory factory;

    try
    {
        m_XMLPlatformUtils = new PlatformUtils;
        m_XMLPlatformUtils->Initialize( );
    }
    catch ( XMLParseException &except ) 
    {
        std::string errMsg = " XML parser init FAILED, reason: ";
        errMsg.append( except.what() );

        wxMessageBox( errMsg
                    , "FATAL Error"
                    , wxOK | wxICON_ERROR );
        delete m_XMLPlatformUtils;
        return false;
    }

    m_DOMParser = factory.Create( );

    m_DOMParser->SetErrorHandler( );

    //  Set parser to start enforcing all the constraints and rules specified
    //  by the NameSpace specification.
    m_DOMParser->setDoNamespaces( true );    

    //  Set parser to process the schema processing.
    m_DOMParser->setDoSchema( false );

    //  Sets parser's behaviour when it encounters a validation constraint
    //  error, the the parser will treat validation error as fatal.
    m_DOMParser->setValidationConstraintFatal( true );
     
    return true;
}


IMPLEMENT_APP( Client )
