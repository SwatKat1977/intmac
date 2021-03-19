//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef ASSETDATA_H
#define ASSETDATA_H

#include <dataModels/iAssetData.h>

typedef std::list<iAssetData *> AssetDataPointerList;


class AssetData : public iAssetData
{
public:
    AssetData( AssetID assetID
             , AssetItemType type
             , std::string title
             , void *data );

    ~AssetData();

    AssetID GetID() { return m_assetID; }

    std::string GetName() { return m_name; }

    void *GetAssetData() { return m_data; }

    AssetItemType GetType() { return m_type; }

    void SetName( std::string name ) { m_name = name; }

    void SetData( void *data ) { m_data = data; }

    void AddChild( iAssetData *newChild );

    void DeleteChild( AssetID id );

    AssetIDList GetChildren();

protected:
    AssetID m_assetID;
    std::string m_name;
    void *m_data;
    AssetItemType m_type;
    AssetDataPointerList m_children;

    void DeleteAllChildren();
};

#endif
