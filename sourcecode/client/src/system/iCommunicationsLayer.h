//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef ICOMMUNICATIONSLAYER_H
#define ICOMMUNICATIONSLAYER_H

#include <system/iCommsLayerConnectInfo.h>


class iCommunicationsLayer
{
public:
    virtual ~iCommunicationsLayer() { ; }

    virtual bool ConnectToServer( iCommsLayerConnectInfo *connInfo ) = 0;

    virtual void DisconnectFromServer() = 0;

    virtual bool IsConnected() = 0;

    virtual int Write( char *data
                     , unsigned int size ) = 0;

    virtual int Read( unsigned int size
                    , char *readBuffer ) = 0;

    virtual void ManageConnectionToServer() = 0;

    virtual bool IsIncomingDataWaiting() = 0;
};

#endif
