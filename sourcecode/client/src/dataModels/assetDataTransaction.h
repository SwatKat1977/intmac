//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef ASSETDATATRANSACTION_H
#define ASSETDATATRANSACTION_H

#include <string>
#include <dataModels/iAssetData.h>


enum AssetDataTransactionType
{
    AssetDataTransactionType_add,
    AssetDataTransactionType_delete,
    AssetDataTransactionType_dataModified,
    AssetDataTransactionType_titleModified

};


class AssetDataTransaction
{
public:
    AssetDataTransaction( AssetID ID
                        , AssetID parentID
                        , std::string title
                        , AssetDataTransactionType transType
                        , iAssetData *data );

    ~AssetDataTransaction();

    AssetID GetID() { return m_ID; }
    
    AssetID GetParentID() { return m_parentID; }

    std::string GetTitle() { return m_title; }

    AssetDataTransactionType GetType() { return m_type; }

    iAssetData *GetAssetData() { return m_asset; }
    
protected:
    std::string m_title;
    AssetID m_ID;
    AssetID m_parentID;
    AssetDataTransactionType m_type;
    iAssetData *m_asset;
};

#endif
