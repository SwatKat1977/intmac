//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <system/shMemHarnessCommsLayer.h>
#include <sharedMemoryFactory.h>
#include <system/ShMemConnectionInfo.h>
#include <MsgProtocolDefs.h>


const int ConnectionTimeoutSeconds = 3;


struct shMemControlBlock
{
    unsigned int m_bufferSize;
};


const int shMemoryClientServerMsgSize = sizeof( shMemControlBlock ) 
                                      + MessageProtocol::MessageHeaderSize
                                      + MessageProtocol::MaxMessageBodySize;


ShMemHarnessCommsLayer::ShMemHarnessCommsLayer()
                      : m_isConnected( false )
                      , m_bytesToRead( 0 )
{
    Shared_Memory::SharedMemoryFactory *shMemFactory = NULL;

    m_shMemAdaptor = new Shared_Memory::SharedMemoryAdaptor;
    shMemFactory = new Shared_Memory::SharedMemoryFactory( m_shMemAdaptor );

    m_shMemoryObject  = shMemFactory->Create();

    delete shMemFactory;
}


ShMemHarnessCommsLayer::~ShMemHarnessCommsLayer()
{
    if ( m_shMemoryObject ) delete m_shMemoryObject;
    if ( m_readSharedMemory ) delete m_readSharedMemory;
    if ( m_writeSharedMemory ) delete m_writeSharedMemory;
    if ( m_shMemAdaptor ) delete m_shMemAdaptor;
}


bool ShMemHarnessCommsLayer::ConnectToServer( iCommsLayerConnectInfo *connInfo )
{
    bool returnStatus = true;
    ShMemConnectionInfo *info = NULL;
    info = static_cast<ShMemConnectionInfo *>( connInfo );

    try
    {
        m_readSharedMemory = m_shMemoryObject->ConnectToSharedMemory( info->GetReadBufferName()
                                                                    , info->GetReadBufferSize() );
        m_writeSharedMemory = m_shMemoryObject->ConnectToSharedMemory( info->GetWriteBufferName()
                                                                     , info->GetWriteBufferSize() );
        m_isConnected = true;
    }
    catch( ... )
    {
        returnStatus = false;
    }

    return returnStatus;
}


void ShMemHarnessCommsLayer::DisconnectFromServer()
{
    if ( m_readSharedMemory ) m_readSharedMemory->Destroy(); 
    if ( m_writeSharedMemory ) m_writeSharedMemory->Destroy(); 

    m_isConnected = false;
}


int ShMemHarnessCommsLayer::Write( char *data
                                 , unsigned int size )
{
    m_writeSharedMemory->Write( data, size, 0 );

    return size;
}


int ShMemHarnessCommsLayer::Read( unsigned int size
                                , char *readBuffer )
{
    unsigned int readBufferSize = 0;
    unsigned int bytesInBuffer = GetReadBufferSize();
    
    if ( m_bytesToRead == 0 ) m_bytesToRead = bytesInBuffer;

    if ( m_bytesToRead == 0 ) return 0;

    readBufferSize = ( m_bytesToRead >= size ) ? size : m_bytesToRead;

    m_readSharedMemory->Read( readBufferSize
                             , sizeof( shMemControlBlock ) 
                               + ( bytesInBuffer - m_bytesToRead )
                             , readBuffer );

    if ( ( m_bytesToRead - readBufferSize ) == 0 )
    {
        shMemControlBlock ctrlBlock;
        ctrlBlock.m_bufferSize = 0;

        m_readSharedMemory->Write( ( char * ) &ctrlBlock
                                 , sizeof( shMemControlBlock )
                                 , 0 );
        m_bytesToRead = 0;
    }
    else
    {
        m_bytesToRead -= readBufferSize;
    }

    return readBufferSize;
}


void ShMemHarnessCommsLayer::ManageConnectionToServer()
{
    if ( IsConnected() == true )
    {
        time_t currentTime = time( NULL );

        if ( ( m_lastMsgTimestamp + ConnectionTimeoutSeconds ) > currentTime )
        {
            DisconnectFromServer();  
        }
    }
}


bool ShMemHarnessCommsLayer::IsIncomingDataWaiting()
{
    return ( GetReadBufferSize() == 0 ) ? false : true;
}


unsigned int ShMemHarnessCommsLayer::GetReadBufferSize()
{
    shMemControlBlock shMemCtrlBlock;

    m_readSharedMemory->Read( sizeof( shMemControlBlock )
                            , 0
                            , ( char * ) &shMemCtrlBlock );

    return shMemCtrlBlock.m_bufferSize;
}
