//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef MOCK_SHAREDMEMORYADAPTOR_H
#define MOCK_SHAREDMEMORYADAPTOR_H

#include <gmock/gmock.h>
#include <iSharedMemoryAdaptor.h>

using namespace Common;

class MockSharedMemoryAdaptor : public Shared_Memory::iSharedMemoryAdaptor
{
public:
    MOCK_METHOD3( Create, HANDLE( DWORD
                                , unsigned long
                                , std::string ) );
    MOCK_METHOD3( OpenSharedMemory, HANDLE( Shared_Memory::SharedMemoryAccess
                                          , unsigned long
                                          , std::string ) );
    MOCK_METHOD3( MapSharedMemory, LPCTSTR( HANDLE
                                          , Shared_Memory::SharedMemoryAccess
                                          , unsigned long ) );
    MOCK_METHOD1( MapSharedMemory, void( LPCTSTR ) );
    MOCK_METHOD1( UnmapShareView, void( LPCTSTR ) );
    MOCK_METHOD1( CloseSharedMemory, void( HANDLE ) );
};

#endif
