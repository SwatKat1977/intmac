//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <sstream>
#include <sharedMemory.h>
#include <sharedMemoryBlock.h>
#include <sharedMemoryConnection.h>


namespace Common
{
namespace Shared_Memory
{


SharedMemory::SharedMemory( iSharedMemoryAdaptor *shMemAdaptor )
             : m_shMemAdaptor( shMemAdaptor )
{
}


SharedMemory::~SharedMemory()
{
}


iSharedMemoryBlock *SharedMemory::CreateSharedMemory( std::string mappedObjectName
                                                    , unsigned long bufferSize )
{
    return new SharedMemoryBlock( m_shMemAdaptor
                                , mappedObjectName
                                , bufferSize );
}


iSharedMemoryBlock *SharedMemory::ConnectToSharedMemory( std::string mappedObjectName 
                                                       , unsigned long bufferSize )
{
    return new SharedMemoryConnection( m_shMemAdaptor
                                     , mappedObjectName 
                                     , bufferSize );
}

}
}
 