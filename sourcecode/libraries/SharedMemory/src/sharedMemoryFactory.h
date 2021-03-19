//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef SHAREDMEMORYFACTORY_H
#define SHAREDMEMORYFACTORY_H

#include <iSharedMemory.h>
#include <iSharedMemoryAdaptor.h>


namespace Common
{
namespace Shared_Memory
{


class SharedMemoryFactory
{
public:
    SharedMemoryFactory( iSharedMemoryAdaptor *shMemAdaptor );

    ~SharedMemoryFactory();

    iSharedMemory *Create();

protected:
    iSharedMemoryAdaptor *m_shMemAdaptor;
};

}
}

#endif
