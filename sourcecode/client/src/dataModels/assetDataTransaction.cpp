//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <dataModels/assetDataTransaction.h>


AssetDataTransaction::AssetDataTransaction( AssetID ID
                                          , AssetID parentID
                                          , std::string title
                                          , AssetDataTransactionType transType
                                          , iAssetData *data )
                                          : m_ID( ID )
                                          , m_parentID( parentID )
                                          , m_title( title )
                                          , m_type( transType )
                                          , m_asset( data )
{
}


AssetDataTransaction::~AssetDataTransaction()
{ 
    if ( m_asset ) delete m_asset; 
}

