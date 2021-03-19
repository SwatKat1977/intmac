//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <mock_SharedMemoryAdaptor.h>
#include <sharedMemoryConnection.h>

using namespace Common::Shared_Memory;

using testing::Return;


class SharedMemoryConnectionTests : public ::testing::Test
{
public:
    SharedMemoryConnectionTests()  
    {
        m_shMemAdaptor = new MockSharedMemoryAdaptor;

    }

    ~SharedMemoryConnectionTests()
    {
        if ( m_shMemoryconnection ) delete m_shMemoryconnection;
        if ( m_shMemAdaptor ) delete m_shMemAdaptor;
    }

protected:
    SharedMemoryConnection *m_shMemoryconnection;
    MockSharedMemoryAdaptor *m_shMemAdaptor;
};


TEST_F( SharedMemoryConnectionTests, construction_ok )
{ 
    HANDLE shMemHandle = new HANDLE;
    char shMemBuffer[256];

    EXPECT_CALL( *m_shMemAdaptor, OpenSharedMemory( testing::_
                                                  , testing::_
                                                  , testing::_ ) )
               .WillRepeatedly( Return( shMemHandle ) );

    EXPECT_CALL( *m_shMemAdaptor, MapSharedMemory( testing::_, testing::_, testing::_ ) )
               .WillRepeatedly( Return( shMemBuffer ) );

    m_shMemoryconnection = new SharedMemoryConnection( m_shMemAdaptor 
                                                     , "TestSharedName"
                                                     , 256 );
    EXPECT_TRUE( m_shMemoryconnection != NULL );
}


TEST_F( SharedMemoryConnectionTests, construction_cannotConnect )
{ 
    HANDLE shMemHandle = NULL;

    EXPECT_CALL( *m_shMemAdaptor, OpenSharedMemory( testing::_
                                                  , testing::_
                                                  , testing::_ ) )
               .WillRepeatedly( Return( shMemHandle ) );

    try
    {
        m_shMemoryconnection = new SharedMemoryConnection( m_shMemAdaptor 
                                                         , "TestSharedName"
                                                         , 256 );
        EXPECT_TRUE( m_shMemoryconnection == NULL );
    }
    catch( IntMacException &except )
    {
        ASSERT_STREQ( "Could not connnect to shared memory (0)."
                    , except.what() );
    }
}


TEST_F( SharedMemoryConnectionTests, construction_cannotMap )
{ 
    HANDLE shMemHandle = new HANDLE;
    char *shMemBuffer = NULL;

    EXPECT_CALL( *m_shMemAdaptor, OpenSharedMemory( testing::_
                                                  , testing::_
                                                  , testing::_ ) )
               .WillRepeatedly( Return( shMemHandle ) );

    EXPECT_CALL( *m_shMemAdaptor, MapSharedMemory( testing::_, testing::_, testing::_ ) )
               .WillRepeatedly( Return( shMemBuffer ) );

    EXPECT_CALL( *m_shMemAdaptor, CloseSharedMemory( testing::_ ) )
               .WillRepeatedly( Return() );

    try
    {
        m_shMemoryconnection = new SharedMemoryConnection( m_shMemAdaptor 
                                                         , "TestSharedName"
                                                         , 256 );
        EXPECT_TRUE( m_shMemoryconnection != NULL );

    }
    catch( IntMacException &except )
    {
        ASSERT_STREQ( "Could not map to shared memory (0)."
                    , except.what() );
    }
}


TEST_F( SharedMemoryConnectionTests, read_ok )
{ 
    HANDLE shMemHandle = new HANDLE;
    char shMemBuffer[256];
    char readBuffer[15];

    EXPECT_CALL( *m_shMemAdaptor, OpenSharedMemory( testing::_
                                                  , testing::_
                                                  , testing::_ ) )
               .WillRepeatedly( Return( shMemHandle ) );

    EXPECT_CALL( *m_shMemAdaptor, MapSharedMemory( testing::_, testing::_, testing::_ ) )
               .WillRepeatedly( Return( shMemBuffer ) );

    m_shMemoryconnection = new SharedMemoryConnection( m_shMemAdaptor 
                                                     , "TestSharedName"
                                                     , 256 );

    m_shMemoryconnection->Read(15, 0, readBuffer );
}



/*
namespace Common
{
namespace Shared_Memory
{

SharedMemoryConnection::SharedMemoryConnection( iSharedMemoryAdaptor *shMemAdaptor
                                              , std::string mappedObjectName 
                                              , DWORD bufferSize )
                       : m_mappedObjectName( mappedObjectName )
                       , m_bufferSize( bufferSize )
                       , m_shMemAdaptor( shMemAdaptor )
{
    m_mapFile = m_shMemAdaptor->OpenSharedMemory( SharedMemoryAccess_all
                       , bufferSize
                       , mappedObjectName );

    if ( m_mapFile == NULL )
    {
        std::stringstream exceptionString;

        exceptionString << "Could not connnect to shared memory ("
                        << GetLastError() << ").";

        throw( IntMacException( ShMemoryExcept_connectFail
                              , exceptionString.str() ) );
    }

    MapSharedMemory();
}


SharedMemoryConnection::~SharedMemoryConnection()
{
}


void SharedMemoryConnection::Write( char *data
                                  , unsigned int size
                                  , unsigned int offset )
{
    if ( size > m_bufferSize )
    {
        std::stringstream exceptionString;

        exceptionString << "Write buffer size ("
                        << size
                        << ") exceeded shared memory buffer size of "
                        << m_bufferSize
                        << " bytes.";

        throw( IntMacException( ShMemoryExcept_invalidBufferSize
                              , exceptionString.str() ) );
    }

    if ( ( size + offset ) > m_bufferSize )
    {
        std::stringstream exceptionString;

        exceptionString << "Offset ("
                        << offset
                        << " bytes) exceeded shared memory buffer size of "
                        << m_bufferSize
                        << " bytes.";

        throw( IntMacException( ShMemoryExcept_invalidBufferSize
                              , exceptionString.str() ) );
    }

    CopyMemory( ( PVOID ) ( m_memoryBuffer + offset )
             , data
             , ( strlen( data ) * sizeof( char ) ) );
}


void SharedMemoryConnection::Read( unsigned int size
                                 , unsigned int offset
                                 , char *readBuffer )

{
    if ( size > m_bufferSize )
    {
        std::stringstream exceptionString;

        exceptionString << "Write buffer size ("
                        << size
                        << ") exceeded shared memory buffer size of "
                        << m_bufferSize
                        << " bytes.";

        throw( IntMacException( ShMemoryExcept_invalidBufferSize
                              , exceptionString.str() ) );
    }

    if ( ( size + offset ) > m_bufferSize )
    {
        std::stringstream exceptionString;

        exceptionString << "Offset ("
                        << offset
                        << " bytes) exceeded shared memory buffer size of "
                        << m_bufferSize
                        << " bytes.";

        throw( IntMacException( ShMemoryExcept_invalidBufferSize
                              , exceptionString.str() ) );
    }

    memcpy( readBuffer
          , ( &m_memoryBuffer[0] )
          , size );
    
    readBuffer[size] = '\0';
}


void SharedMemoryConnection::Destroy()
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


void SharedMemoryConnection::MapSharedMemory()
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

*/