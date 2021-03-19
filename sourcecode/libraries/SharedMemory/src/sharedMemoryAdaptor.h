//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef SHAREDMEMORYADAPTOR_H
#define SHAREDMEMORYADAPTOR_H

#include <iSharedMemoryAdaptor.h>


namespace Common
{
namespace Shared_Memory
{

class SharedMemoryAdaptor : public iSharedMemoryAdaptor
{
public:
    SharedMemoryAdaptor();

    ~SharedMemoryAdaptor();

    HANDLE Create( DWORD access
                 , unsigned long size
                 , std::string mapName );

    HANDLE OpenSharedMemory( SharedMemoryAccess access
                           , unsigned long size
                           , std::string mapName );

    LPCTSTR MapSharedMemory( HANDLE mappedObject 
                           , SharedMemoryAccess access
                           , unsigned long size );

    void UnmapShareView( LPCTSTR memoryBuffer );

    void CloseSharedMemory( HANDLE mapFile );
};

}
}

#endif
