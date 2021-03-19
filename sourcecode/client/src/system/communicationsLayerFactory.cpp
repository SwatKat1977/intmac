//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <system/communicationsLayerFactory.h>
#include <system/shMemHarnessCommsLayer.h>


CommunicationsLayerFactory::CommunicationsLayerFactory()
{
}


CommunicationsLayerFactory::~CommunicationsLayerFactory()
{
}


iCommunicationsLayer *CommunicationsLayerFactory::Create( CommsLayerType commsType )
{

    switch( commsType )
    {
    case CommsLayerType_IPV4Network:
        return NULL;
        break;
    case CommsLayerType_sharedMemory:
        return new ShMemHarnessCommsLayer;
        break;

    default:
        return NULL;
    }
}
