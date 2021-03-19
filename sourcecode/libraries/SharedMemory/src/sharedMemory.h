//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <iSharedMemory.h>
#include <iSharedMemoryAdaptor.h>


namespace Common
{
namespace Shared_Memory
{

const std::string Version = "0.1.0";


class SharedMemory : public iSharedMemory
{
public:
    SharedMemory( iSharedMemoryAdaptor *shMemAdaptor );

    ~SharedMemory();

    iSharedMemoryBlock *CreateSharedMemory( std::string mappedObjectName
                                          , unsigned long bufferSize );

    iSharedMemoryBlock *ConnectToSharedMemory( std::string mappedObjectName
                                             , unsigned long bufferSize );

protected:
    iSharedMemoryAdaptor *m_shMemAdaptor;
};

}
}

#endif
