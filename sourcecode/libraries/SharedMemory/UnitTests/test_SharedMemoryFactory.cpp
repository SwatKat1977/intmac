//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <mock_SharedMemoryAdaptor.h>
#include <sharedMemoryFactory.h>

using namespace Common::Shared_Memory;


class SharedMemoryFactoryTests : public ::testing::Test
{
public:
    SharedMemoryFactoryTests()  {}

    ~SharedMemoryFactoryTests()
    {
        if ( m_shMemFactory ) delete m_shMemFactory;
    }

protected:
    SharedMemoryFactory *m_shMemFactory;
};


TEST_F( SharedMemoryFactoryTests, create )
{ 
    MockSharedMemoryAdaptor *shMemAdaptor = NULL;
    m_shMemFactory = NULL;
    iSharedMemory *sharedMem = NULL;

    shMemAdaptor = new MockSharedMemoryAdaptor;
    m_shMemFactory = new SharedMemoryFactory( shMemAdaptor );

    sharedMem = m_shMemFactory->Create();

    EXPECT_TRUE( sharedMem != NULL );
}
