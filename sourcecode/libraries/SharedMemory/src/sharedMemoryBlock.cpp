//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <sstream>
#include <sharedMemoryBlock.h>
#include <sharedMemoryExceptions.h>


namespace Common
{
namespace Shared_Memory
{


SharedMemoryBlock::SharedMemoryBlock( iSharedMemoryAdaptor *shMemAdaptor
                                    , std::string mappedObjectName 
                                    , DWORD bufferSize )
                                    : m_mappedObjectName( mappedObjectName )
                                    , m_bufferSize( bufferSize )
                                    , m_memoryBuffer( NULL )
                                    , m_shMemAdaptor( shMemAdaptor )
{
    m_mapFile = m_shMemAdaptor->Create( CreateMapAccess_readWrite
                                      , bufferSize
                                      , mappedObjectName );

    if ( m_mapFile == NULL )
    {
        std::stringstream exceptionString;

        exceptionString << "Could not create file mapping object ("
                        << GetLastError() << ").";

        throw( IntMacException( ShMemoryExcept_createFail
                              , exceptionString.str() ) );
    }

    MapSharedMemory();
}


SharedMemoryBlock::~SharedMemoryBlock()
{
}


void SharedMemoryBlock::Write( char *data
                             , unsigned int size
                             , unsigned int offset )
{
    if ( size > m_bufferSize ) return;

    if ( ( size + offset ) > m_bufferSize ) return;

    memcpy( ( PVOID ) ( m_memoryBuffer + offset ), data, size );
}


void SharedMemoryBlock::Read( unsigned int size
                            , unsigned int offset
                            , char *readBuffer )

{
    if ( size > m_bufferSize ) return;

    if ( ( size + offset ) > m_bufferSize ) return;

    memcpy( readBuffer
          , ( &m_memoryBuffer[0] ) + offset
          , size );
}


void SharedMemoryBlock::Destroy()
{
    if ( m_mapFile )
    {
        m_shMemAdaptor->CloseSharedMemory( m_mapFile );
        m_mapFile = NULL;
    }

    if ( m_memoryBuffer )
    {
        m_shMemAdaptor->UnmapShareView( m_memoryBuffer );
        m_memoryBuffer = NULL;
    }
}


void SharedMemoryBlock::MapSharedMemory()
{
    m_memoryBuffer = m_shMemAdaptor->MapSharedMemory( m_mapFile
                                                    , SharedMemoryAccess_all
                                                    , m_bufferSize );

    if ( m_memoryBuffer == NULL )
    {
        std::stringstream exceptionString;

        exceptionString << "Could not map to shared memory ("
                        << GetLastError() << ").";

        Destroy();
        throw( IntMacException( ShMemoryExcept_mappingFail
                              , exceptionString.str() ) );
   }
}

}
}
