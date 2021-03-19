//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef SHMEMHARNESSCOMMSLAYER_H
#define SHMEMHARNESSCOMMSLAYER_H

#include <time.h>
#include <system/iCommunicationsLayer.h>
#include <iSharedMemoryBlock.h>
#include <iSharedMemory.h>
#include <sharedMemoryAdaptor.h>


using namespace Common;


class ShMemHarnessCommsLayer : public iCommunicationsLayer
{
public:
    ShMemHarnessCommsLayer();

    ~ShMemHarnessCommsLayer();

    bool ConnectToServer( iCommsLayerConnectInfo *connInfo );

    void DisconnectFromServer();

    bool IsConnected() { return m_isConnected; }

    int Write( char *data
             , unsigned int size );

    int Read( unsigned int size
            , char *readBuffer );

    void ManageConnectionToServer();

    bool IsIncomingDataWaiting();

protected:
    Shared_Memory::iSharedMemory *m_shMemoryObject;
    Shared_Memory::iSharedMemoryBlock *m_readSharedMemory;
    Shared_Memory::iSharedMemoryBlock *m_writeSharedMemory;
    Shared_Memory::SharedMemoryAdaptor *m_shMemAdaptor;
    bool m_isConnected;
    time_t m_lastMsgTimestamp;
    unsigned int m_bytesToRead;

    unsigned int GetReadBufferSize();
};

#endif
