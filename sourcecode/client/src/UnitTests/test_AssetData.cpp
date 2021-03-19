//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <dataModels\assetData.h>
#include <Mock_AssetData.h>


class AssetDataTests : public ::testing::Test
{
public:
    AssetDataTests()  {}

    ~AssetDataTests()
    {
        if ( m_assetData ) delete m_assetData;
    }

protected:
    AssetData *m_assetData;
};


TEST_F( AssetDataTests, Construction_TestGroup )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_testGroup
                               , "TestGroup_Data"
                               , NULL );

    EXPECT_EQ( 10, m_assetData->GetID() );
    EXPECT_EQ( AssetItemType_testGroup, m_assetData->GetType() );
    EXPECT_STREQ( "TestGroup_Data", m_assetData->GetName().c_str() );
    EXPECT_EQ( NULL, m_assetData->GetAssetData() );
}


TEST_F( AssetDataTests, Construction_Filter )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_filter
                               , "Filter_Data"
                               , NULL );

    EXPECT_EQ( 10, m_assetData->GetID() );
    EXPECT_EQ( AssetItemType_filter, m_assetData->GetType() );
    EXPECT_STREQ( "Filter_Data", m_assetData->GetName().c_str() );
    EXPECT_EQ( NULL, m_assetData->GetAssetData() );
}


TEST_F( AssetDataTests, Construction_TestItem )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_testItem
                               , "TestItem_Data"
                               , NULL );

    EXPECT_EQ( 10, m_assetData->GetID() );
    EXPECT_EQ( AssetItemType_testItem, m_assetData->GetType() );
    EXPECT_STREQ( "TestItem_Data", m_assetData->GetName().c_str() );
    EXPECT_EQ( NULL, m_assetData->GetAssetData() );
}


TEST_F( AssetDataTests, SetName )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    m_assetData->SetName( "Construction_SetName" );

    EXPECT_STREQ( "Construction_SetName", m_assetData->GetName().c_str() );
}


TEST_F( AssetDataTests, AddChild_filter )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    AssetData *childAsset = new AssetData( 101
                                         , AssetItemType_filter
                                         , "ChildEntry"
                                         , NULL );

    m_assetData->AddChild( childAsset );

    AssetIDList children = m_assetData->GetChildren();

    ASSERT_EQ( 1, children.size() );

    AssetID front = children.front();

    EXPECT_EQ( 101, front );
}


TEST_F( AssetDataTests, AddChild_asset )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    AssetData *childAsset = new AssetData( 101
                                         , AssetItemType_testGroup
                                         , "ChildEntry"
                                         , NULL );

    m_assetData->AddChild( childAsset );

    AssetIDList children = m_assetData->GetChildren();

    ASSERT_EQ( 1, children.size() );

    AssetID front = children.front();

    EXPECT_EQ( 101, front );
}


TEST_F( AssetDataTests, AddChild_duplicateID )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    AssetData *childAsset1 = new AssetData( 101
                                          , AssetItemType_testGroup
                                          , "ChildEntry1"
                                          , NULL );

    AssetData *childAsset2 = new AssetData( 102
                                          , AssetItemType_filter
                                          , "ChildEntry2"
                                          , NULL );

    m_assetData->AddChild( childAsset1 );
    ASSERT_EQ( 1, m_assetData->GetChildren().size() );
    AssetID front = m_assetData->GetChildren().front();
    EXPECT_EQ( 101, front );

    m_assetData->AddChild( childAsset2 );

    ASSERT_ANY_THROW( m_assetData->AddChild( childAsset1 ) );

    EXPECT_EQ( 2, m_assetData->GetChildren().size() );
}


TEST_F( AssetDataTests, DeleteChild_invalidID )
{ 
    AssetID frontAssetID;
    m_assetData = new AssetData( 10
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    AssetData *childAsset = new AssetData( 101
                                         , AssetItemType_filter
                                         , "ChildEntry"
                                         , NULL );

    m_assetData->AddChild( childAsset );
     
    AssetIDList firstChildList = m_assetData->GetChildren();
    ASSERT_EQ( 1, firstChildList.size() );
    frontAssetID = firstChildList.front();
    EXPECT_EQ( 101, frontAssetID );

    m_assetData->DeleteChild( 202 );

    AssetIDList secondChildList = m_assetData->GetChildren();
    ASSERT_EQ( 1, secondChildList.size() );
    frontAssetID = secondChildList.front();
    EXPECT_EQ( 101, frontAssetID );
}


TEST_F( AssetDataTests, DeleteChild_validAssetID )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    AssetData *childAsset1 = new AssetData( 101
                                          , AssetItemType_filter
                                          , "ChildEntry1"
                                          , NULL );

    AssetData *childAsset2 = new AssetData( 102
                                          , AssetItemType_filter
                                          , "ChildEntry2"
                                          , NULL );

    m_assetData->AddChild( childAsset1 );
    m_assetData->AddChild( childAsset2 );
     
    EXPECT_EQ( 2, m_assetData->GetChildren().size() );

    m_assetData->DeleteChild( 101 );

    ASSERT_EQ( 1, m_assetData->GetChildren().size() );
}


TEST_F( AssetDataTests, DeleteChild_testItemType )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    AssetData *childAsset1 = new AssetData( 101
                                          , AssetItemType_testItem
                                          , "ChildEntry1"
                                          , NULL );

    AssetData *childAsset2 = new AssetData( 102
                                          , AssetItemType_testItem
                                          , "ChildEntry2"
                                          , NULL );

    m_assetData->AddChild( childAsset1 );
    m_assetData->AddChild( childAsset2 );
     
    EXPECT_EQ( 2, m_assetData->GetChildren().size() );

    m_assetData->DeleteChild( 101 );

    ASSERT_EQ( 1, m_assetData->GetChildren().size() );
}


TEST_F( AssetDataTests, DeleteChild_validFilterID )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    AssetData *childAsset1 = new AssetData( 101
                                          , AssetItemType_testGroup
                                          , "ChildEntry1"
                                          , NULL );

    AssetData *childAsset2 = new AssetData( 102
                                          , AssetItemType_filter
                                          , "ChildEntry2"
                                          , NULL );

    m_assetData->AddChild( childAsset1 );
    m_assetData->AddChild( childAsset2 );
     
    EXPECT_EQ( 2, m_assetData->GetChildren().size() );

    m_assetData->DeleteChild( 102 );

    ASSERT_EQ( 1, m_assetData->GetChildren().size() );
}


TEST_F( AssetDataTests, DeleteChild_validUnknownType )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    AssetData *childAsset1 = new AssetData( 101
                                          , (AssetItemType) 12323
                                          , "ChildEntry1"
                                          , NULL );

    m_assetData->AddChild( childAsset1 );
     
    EXPECT_EQ( 1, m_assetData->GetChildren().size() );

    m_assetData->DeleteChild( 101 );

    ASSERT_EQ( 1, m_assetData->GetChildren().size() );
}


TEST_F( AssetDataTests, SetData_placeholder )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    m_assetData->SetData( NULL );

    ASSERT_EQ( NULL, m_assetData->GetAssetData() );
}


TEST_F( AssetDataTests, DeleteAllChildren_filtersOnly )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    AssetData *child = new AssetData( 20
                                    , AssetItemType_filter
                                    , "TestAssetData"
                                    , NULL );

    AssetData *childAsset1 = new AssetData( 101
                                          , AssetItemType_filter
                                          , "ChildEntry1"
                                          , NULL );

    AssetData *childAsset2 = new AssetData( 102
                                          , AssetItemType_filter
                                          , "ChildEntry2"
                                          , NULL );

    AssetData *childAsset3 = new AssetData( 103
                                          , AssetItemType_filter
                                          , "ChildEntry3"
                                          , NULL );

    m_assetData->AddChild( child );

    child->AddChild( childAsset1 );
    child->AddChild( childAsset2 );
    child->AddChild( childAsset3 );

    EXPECT_EQ( 3, child->GetChildren().size() );

    m_assetData->DeleteChild( 20 );

    EXPECT_EQ( 0, child->GetChildren().size() );
}


TEST_F( AssetDataTests, DeleteAllChildren_testGroupsOnly )
{ 
    m_assetData = new AssetData( 10 
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    AssetData *child = new AssetData( 20
                                    , AssetItemType_filter
                                    , "TestAssetData"
                                    , NULL );

    AssetData *childAsset1 = new AssetData( 101
                                          , AssetItemType_testGroup
                                          , "ChildEntry1"
                                          , NULL );

    AssetData *childAsset2 = new AssetData( 102
                                          , AssetItemType_testGroup
                                          , "ChildEntry2"
                                          , NULL );

    AssetData *childAsset3 = new AssetData( 103
                                          , AssetItemType_testGroup
                                          , "ChildEntry3"
                                          , NULL );

    m_assetData->AddChild( child );

    child->AddChild( childAsset1 );
    child->AddChild( childAsset2 );
    child->AddChild( childAsset3 );

    EXPECT_EQ( 3, child->GetChildren().size() );

    m_assetData->DeleteChild( 20 );

    EXPECT_EQ( 0, child->GetChildren().size() );
}


TEST_F( AssetDataTests, DeleteAllChildren_testItemOnly )
{ 
    m_assetData = new AssetData( 10 
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    AssetData *child = new AssetData( 20
                                    , AssetItemType_filter
                                    , "TestAssetData"
                                    , NULL );

    AssetData *childAsset1 = new AssetData( 101
                                          , AssetItemType_testItem
                                          , "ChildEntry1"
                                          , NULL );

    AssetData *childAsset2 = new AssetData( 102
                                          , AssetItemType_testItem
                                          , "ChildEntry2"
                                          , NULL );

    AssetData *childAsset3 = new AssetData( 103
                                          , AssetItemType_testItem
                                          , "ChildEntry3"
                                          , NULL );

    m_assetData->AddChild( child );

    child->AddChild( childAsset1 );
    child->AddChild( childAsset2 );
    child->AddChild( childAsset3 );

    EXPECT_EQ( 3, child->GetChildren().size() );

    m_assetData->DeleteChild( 20 );

    EXPECT_EQ( 0, child->GetChildren().size() );
}


TEST_F( AssetDataTests, DeleteAllChildren_mixedChildren )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    AssetData *child = new AssetData( 20
                                    , AssetItemType_filter
                                    , "TestAssetData"
                                    , NULL );

    AssetData *childAsset1 = new AssetData( 101
                                          , AssetItemType_testGroup
                                          , "ChildEntry1"
                                          , NULL );

    AssetData *childAsset2 = new AssetData( 102
                                          , AssetItemType_filter
                                          , "ChildEntry2"
                                          , NULL );

    AssetData *childAsset3 = new AssetData( 103
                                          , AssetItemType_filter
                                          , "ChildEntry3"
                                          , NULL );

    AssetData *childAsset4 = new AssetData( 104
                                          , AssetItemType_testGroup
                                          , "ChildEntry4"
                                          , NULL );

    m_assetData->AddChild( child );

    child->AddChild( childAsset1 );
    child->AddChild( childAsset2 );
    child->AddChild( childAsset3 );
    child->AddChild( childAsset4 );

    EXPECT_EQ( 4, child->GetChildren().size() );

    m_assetData->DeleteChild( 20 );

    EXPECT_EQ( 0, child->GetChildren().size() );
}


TEST_F( AssetDataTests, DeleteAllChildren_invalidType )
{ 
    m_assetData = new AssetData( 10
                               , AssetItemType_filter
                               , "TestAssetData"
                               , NULL );

    AssetData *child = new AssetData( 20
                                    , AssetItemType_filter
                                    , "TestAssetData"
                                    , NULL );

    AssetData *childAsset1 = new AssetData( 101
                                          , (AssetItemType) 2609
                                          , "ChildEntry1"
                                          , NULL );

    m_assetData->AddChild( child );

    child->AddChild( childAsset1 );

    EXPECT_EQ( 1, child->GetChildren().size() );

    m_assetData->DeleteChild( 20 );

    EXPECT_EQ( 1, child->GetChildren().size() );
}
