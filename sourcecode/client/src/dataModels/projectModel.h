//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <string>
#include <map>
#include <MVP/iModel.h>
#include <system/iMessageHandler.h>
#include <dataModels/assetData.h>
#include <dataModels/releaseData.h>
#include <dataModels/assetDataTransaction.h>
#include <dataModels/releaseTransaction.h>


class ProjectModel : public iModel
{
public:
    ProjectModel();

    ~ProjectModel();

    void SetMessageHandler( Comms::iMessageHandler *msgHandler );

    //  +++++++++++++++++
    //  + Data Setters
    //  ++++++++++++++++
    bool AddAsset( AssetID assetID
                 , AssetID parentID
                 , std::string name
                 , AssetItemType type );

    void AddBulkProjectAssetsData( ProjectAssetsList &assets );
    
     bool AddRelease( ReleaseItemID ID
                    , ReleaseItemID parentID
                    , std::string name
                    , ReleaseItemType type );

    void AddBulkReleasesAssetsData( ReleaseEntriesList &assets );


    //  +++++++++++++++++
    //  + Data Getters
    //  +++++++++++++++++
    AssetDataTransaction *GetTopAssetTransaction();
    size_t GetNumberOfAssetTransactions() { return m_assetTransactions.size(); }
    void PopTopAssetTransaction();

    ReleaseTransaction *GetTopReleaseTransaction();
    size_t GetNumberOfReleaseTransactions() { return m_releaseTransactions.size(); }
    void PopTopReleaseTransaction();

protected:
    Comms::iMessageHandler *m_msgHandler;
    AssetData *m_assetDataRoot;
    ReleaseData *m_releaseDataRoot;
    std::map<AssetID, AssetData *> m_assetsData;
    std::map<ReleaseItemID, ReleaseData *> m_releasesData;
    std::list<AssetDataTransaction *> m_assetTransactions;
    std::list<ReleaseTransaction *> m_releaseTransactions;
};

#endif
