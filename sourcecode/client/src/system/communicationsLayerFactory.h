//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef COMMUNICATIONSLAYERFACTORY_H
#define COMMUNICATIONSLAYERFACTORY_H

#include <system/iCommunicationsLayer.h>

enum CommsLayerType
{
    CommsLayerType_IPV4Network,
    CommsLayerType_sharedMemory
};


class CommunicationsLayerFactory 
{
public:
    CommunicationsLayerFactory();

    ~CommunicationsLayerFactory();

    iCommunicationsLayer *Create( CommsLayerType commsType );
};

#endif
