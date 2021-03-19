//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <mock_SharedMemoryAdaptor.h>
#include <sharedMemory.h>

using namespace Common::Shared_Memory;

using testing::Return;


class SharedMemoryTests : public ::testing::Test
{
public:
    SharedMemoryTests()  
    {
        m_shMemAdaptor = new MockSharedMemoryAdaptor;

    }

    ~SharedMemoryTests()
    {
        if ( m_sharedMemory ) delete m_sharedMemory;
        if ( m_shMemAdaptor ) delete m_shMemAdaptor;
    }

protected:
    SharedMemory *m_sharedMemory;
    MockSharedMemoryAdaptor *m_shMemAdaptor;
};


TEST_F( SharedMemoryTests, createSharedMemory )
{ 
    m_sharedMemory = new SharedMemory( m_shMemAdaptor );
    iSharedMemoryBlock *shMemBlock = NULL;
    char *tmpHANDLE = new char[10];

    EXPECT_CALL( *m_shMemAdaptor, Create( testing::_, testing::_, testing::_ ) )
        .WillRepeatedly( Return( tmpHANDLE ) );

    EXPECT_CALL( *m_shMemAdaptor, MapSharedMemory( testing::_, testing::_, testing::_ ) )
        .WillRepeatedly( Return( tmpHANDLE ) );

    shMemBlock = m_sharedMemory->CreateSharedMemory( "TEST101", 100 );

    EXPECT_TRUE( shMemBlock != NULL );
}


TEST_F( SharedMemoryTests, connectToSharedMemory )
{ 
    m_sharedMemory = new SharedMemory( m_shMemAdaptor );
    iSharedMemoryBlock *shMemBlock = NULL;
    char *tmpHANDLE = new char[10];

    EXPECT_CALL( *m_shMemAdaptor, OpenSharedMemory( testing::_
                                                  , testing::_
                                                  , testing::_ ) )
               .WillRepeatedly( Return( tmpHANDLE ) );

    EXPECT_CALL( *m_shMemAdaptor, MapSharedMemory( testing::_, testing::_, testing::_ ) )
        .WillRepeatedly( Return( tmpHANDLE ) );

    shMemBlock = m_sharedMemory->ConnectToSharedMemory( "TEST101", 100 );

    EXPECT_TRUE( shMemBlock != NULL );
}
