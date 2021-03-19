//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef SHAREDMEMORYCONNECTION_H
#define SHAREDMEMORYCONNECTION_H

#include <Windows.h>
#include <iSharedMemoryBlock.h>
#include <IntMacException.h>
#include <iSharedMemoryAdaptor.h>

namespace Common
{
namespace Shared_Memory
{

class SharedMemoryConnection : public iSharedMemoryBlock
{
public:
    SharedMemoryConnection( iSharedMemoryAdaptor *shMemAdaptor
                          , std::string mappedObjectName
                          , DWORD bufferSize );

    ~SharedMemoryConnection();

    void Read( unsigned int size
             , unsigned int offset
             , char *readBuffer );

    void Write( char *data
              , unsigned int size
              , unsigned int offset );

    void Destroy();

protected:
    std::string m_mappedObjectName;
    HANDLE m_mapFile;
    DWORD m_bufferSize;
    LPCTSTR m_memoryBuffer;
    iSharedMemoryAdaptor *m_shMemAdaptor;

    void MapSharedMemory();
};

}
}

#endif
