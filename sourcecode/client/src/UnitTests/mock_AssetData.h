//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef MOCK_ASSETDATA_H
#define MOCK_ASSETDATA_H

#include <gmock/gmock.h>
#include <dataModels\iAssetData.h>

class MockAssetData : public iAssetData
{
public:
    MOCK_METHOD0( GetID, AssetID() );
    MOCK_METHOD0( GetName, std::string() );
    MOCK_METHOD0( GetAssetData, void *() );
    MOCK_METHOD0( GetType, AssetItemType() );
    MOCK_METHOD1( SetName, void( std::string ) );
    MOCK_METHOD1( SetData, void( void *data ) );
    MOCK_METHOD1( AddChild, void( iAssetData * ) );
    MOCK_METHOD1( DeleteChild, void( AssetID ) );
    MOCK_METHOD0( GetChildren, AssetIDList() );
    MOCK_METHOD0( DeleteAllChildrens, void() );
};


#endif
