//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <system/MessageTriggers.h>
#include <MessageTriggerManager.h>

using namespace Common;


class MessageTriggerManagerTests : public ::testing::Test
{
public:
    MessageTriggerManagerTests()  {}

    ~MessageTriggerManagerTests()
    {
        if ( m_triggerManager ) delete m_triggerManager;
    }

protected:
    MessageTriggerManager *m_triggerManager;
};


TEST_F( MessageTriggerManagerTests, Construction )
{ 
    Common::TriggersList triggersList = { MsgTrigger_handshakeCompleted
                                        , MsgTrigger_userAuthorised
                                        , MsgTrigger_projectListBulkStarted
                                        };

    m_triggerManager = new MessageTriggerManager( triggersList );
}


TEST_F( MessageTriggerManagerTests, Trigger_invalidTrigger )
{ 
    Common::TriggersList triggersList = { MsgTrigger_handshakeCompleted
                                        , MsgTrigger_userAuthorised
                                        , MsgTrigger_projectListBulkStarted
                                        };

    m_triggerManager = new MessageTriggerManager( triggersList );

    m_triggerManager->Trigger( 9001 );

    EXPECT_EQ( false, m_triggerManager->IsTriggered( 9001 ) );
}


TEST_F( MessageTriggerManagerTests, Trigger_validTrigger )
{ 
    Common::TriggersList triggersList = { MsgTrigger_handshakeCompleted
                                        , MsgTrigger_userAuthorised
                                        , MsgTrigger_projectListBulkStarted
                                        };

    m_triggerManager = new MessageTriggerManager( triggersList );

    m_triggerManager->Trigger( MsgTrigger_userAuthorised );

    EXPECT_EQ( true, m_triggerManager->IsTriggered( MsgTrigger_userAuthorised ) );
}


TEST_F( MessageTriggerManagerTests, Untrigger_invalidTrigger )
{ 
    Common::TriggersList triggersList = { MsgTrigger_handshakeCompleted
                                        , MsgTrigger_userAuthorised
                                        , MsgTrigger_projectListBulkStarted
                                        };

    m_triggerManager = new MessageTriggerManager( triggersList );

    m_triggerManager->Untrigger( 9001 );

    EXPECT_EQ( false, m_triggerManager->IsTriggered( 9001 ) );
}


TEST_F( MessageTriggerManagerTests, Untrigger_validTrigger )
{ 
    Common::TriggersList triggersList = { MsgTrigger_handshakeCompleted
                                        , MsgTrigger_userAuthorised
                                        , MsgTrigger_projectListBulkStarted
                                        };

    m_triggerManager = new MessageTriggerManager( triggersList );

    m_triggerManager->Trigger( MsgTrigger_userAuthorised );
    EXPECT_EQ( true, m_triggerManager->IsTriggered( MsgTrigger_userAuthorised ) );

    m_triggerManager->Untrigger( MsgTrigger_userAuthorised );
    EXPECT_EQ( false, m_triggerManager->IsTriggered( MsgTrigger_userAuthorised ) );
}
