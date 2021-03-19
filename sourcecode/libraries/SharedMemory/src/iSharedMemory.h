//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef ISHAREDMEMORY_H
#define ISHAREDMEMORY_H

#include <string>
#include <iSharedMemoryBlock.h>


namespace Common
{
namespace Shared_Memory
{

class iSharedMemory
{
public:

    virtual ~iSharedMemory() {}

    virtual iSharedMemoryBlock *CreateSharedMemory( std::string mappedObjectName
                                                  , unsigned long bufferSize ) = 0;

    virtual iSharedMemoryBlock *ConnectToSharedMemory( std::string mappedObjectName
                                                     , unsigned long bufferSize ) = 0;
};

}
}

#endif
