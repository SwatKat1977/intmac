//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <sharedMemoryAdaptor.h>

namespace Common
{
namespace Shared_Memory
{


SharedMemoryAdaptor::SharedMemoryAdaptor()
{
}


SharedMemoryAdaptor::~SharedMemoryAdaptor()
{
}


HANDLE SharedMemoryAdaptor::Create( DWORD access
                                  , unsigned long size
                                  , std::string mapName )
{
    HANDLE shaMemorymapFile = NULL;

    shaMemorymapFile = ::CreateFileMapping( INVALID_HANDLE_VALUE
                                          , NULL
                                          , access
                                          , 0
                                          , size
                                          , mapName.c_str() );

    return shaMemorymapFile;
}


LPCTSTR SharedMemoryAdaptor::MapSharedMemory( HANDLE mappedObject 
                                            , SharedMemoryAccess access
                                            , unsigned long size )
{
    LPCTSTR viewHandle;
    DWORD accessLevel = 0;

    switch( access )
    {
    case SharedMemoryAccess_write:
        accessLevel = FILE_MAP_WRITE;
        break;

    case SharedMemoryAccess_read:
        accessLevel = FILE_MAP_READ;
        break;

    case SharedMemoryAccess_all:
        accessLevel = FILE_MAP_ALL_ACCESS;
        break;

    default:
        throw ( std::string( "Invalid access value!" ) );
        break;
    }

    if ( mappedObject == NULL ) 
    {
        throw( std::string( "Invalid mapped object passed to ::MapSharedMemory()") );
    }

    viewHandle = (LPTSTR) ::MapViewOfFile( mappedObject
                                         , accessLevel
                                         , 0
                                         , 0
                                         , size );
    
    return viewHandle;
}


HANDLE SharedMemoryAdaptor::OpenSharedMemory( SharedMemoryAccess access
                                            , unsigned long size
                                            , std::string mapName )
{
    HANDLE shMemory = NULL;
    DWORD accessLevel = 0;

    switch( access )
    {
    case SharedMemoryAccess_write:
        accessLevel = FILE_MAP_WRITE;
        break;

    case SharedMemoryAccess_read:
        accessLevel = FILE_MAP_READ;
        break;

    case SharedMemoryAccess_all:
        accessLevel = FILE_MAP_ALL_ACCESS;
        break;

    default:
        throw ( std::string( "Invalid access value!" ) );
        break;
    }

    shMemory = OpenFileMapping( accessLevel
                              , FALSE
                              , mapName.c_str() );

    return shMemory;
}


void SharedMemoryAdaptor::UnmapShareView( LPCTSTR memoryBuffer )
{
    if ( memoryBuffer == NULL ) return;
 
    UnmapViewOfFile( memoryBuffer );
}


void SharedMemoryAdaptor::CloseSharedMemory( HANDLE mapFile )
{
    if ( mapFile == NULL ) return;
 
    CloseHandle( mapFile );
}

}
}
