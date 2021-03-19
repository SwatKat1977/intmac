//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <dataModels/projectModel.h>

const AssetID AssetDataRootID = 0x0;
const ReleaseItemID ReleaseDataRootID = 0x0;


ProjectModel::ProjectModel()
            : m_msgHandler( nullptr ) 
{

    m_assetDataRoot = new AssetData( AssetDataRootID
                                   , AssetItemType_filter
                                   , "<ROOT>"
                                   , NULL );
    
    m_assetsData[AssetDataRootID] = m_assetDataRoot;

    m_releaseDataRoot = new ReleaseData( ReleaseDataRootID
                                       , ReleaseItemType_folder
                                       , "<ROOT>"
                                       , NULL );

    m_releasesData[ReleaseDataRootID] = m_releaseDataRoot;
}


ProjectModel::~ProjectModel()
{
    if ( m_assetDataRoot ) delete m_assetDataRoot; 
}


bool ProjectModel::AddAsset( AssetID assetID
                           , AssetID parentID
                           , std::string name
                           , AssetItemType type )
{
    AssetData *newFilter = NULL;

    if ( m_assetsData.find( parentID ) == m_assetsData.end() )
    {
        return false; 
    }

    newFilter = new AssetData( assetID
                             , type
                             , name
                             , NULL );
    m_assetsData[parentID]->AddChild( newFilter );
    m_assetsData[assetID] = newFilter;

    AssetData *assetDataEntry = new AssetData( assetID
                                             , type
                                             , name
                                             , NULL );
    AssetDataTransaction *newTransaction = new AssetDataTransaction( assetID
                                                                   , parentID
                                                                   , name
                                                                   , AssetDataTransactionType_add
                                                                   , assetDataEntry );

    m_assetTransactions.push_back( newTransaction );

    return true;
}


void ProjectModel::AddBulkProjectAssetsData( ProjectAssetsList &assets )
{
    ProjectAssetsList::iterator it;

    for ( it = assets.begin(); it != assets.end(); it++ )
    {
        AddAsset( it->m_id, it->m_parentID, it->m_name, it->m_type );
    }
}


AssetDataTransaction *ProjectModel::GetTopAssetTransaction()
{
    if ( m_assetTransactions.size() == 0 ) return NULL;

    return m_assetTransactions.front();
}


void ProjectModel::PopTopAssetTransaction()
{
    if ( m_assetTransactions.size() == 0 ) return;

    delete *( m_assetTransactions.begin() );

    m_assetTransactions.pop_front();
}


void ProjectModel::SetMessageHandler( Comms::iMessageHandler *msgHandler )
{
    m_msgHandler = msgHandler;
}


bool ProjectModel::AddRelease( ReleaseItemID ID
                             , ReleaseItemID parentID
                             , std::string name
                             , ReleaseItemType type )
{
    if ( m_releasesData.find( parentID ) == m_releasesData.end() )
    {
        return false; 
    }

    ReleaseData *releaseData = new ReleaseData( ID
                                              , type
                                              , name
                                              , NULL );
    m_releasesData[parentID]->AddChild( releaseData );
    m_releasesData[ID] = releaseData;

    ReleaseTransaction *transaction = new ReleaseTransaction( ID
                                                            , parentID
                                                            , name
                                                            , ReleaseTransactionType_add
                                                            , type );

    m_releaseTransactions.push_back( transaction );

    return true;
}


void ProjectModel::AddBulkReleasesAssetsData( ReleaseEntriesList &assets )
{
    ReleaseEntriesList::iterator it;

    for ( it = assets.begin(); it != assets.end(); it++ )
    {
        AddRelease( it->m_id, it->m_parentID, it->m_name, it->m_type );
    }
}


ReleaseTransaction *ProjectModel::GetTopReleaseTransaction()
{
    
    if ( m_releaseTransactions.size() == 0 ) return NULL;

    return m_releaseTransactions.front();
}


void ProjectModel::PopTopReleaseTransaction()
{
    if ( m_releaseTransactions.size() == 0 ) return;

    delete *( m_releaseTransactions.begin() );

    m_releaseTransactions.pop_front();
}
