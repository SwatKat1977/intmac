//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <dataModels/assetData.h>


AssetData::AssetData( AssetID assetID
                    , AssetItemType type
                    , std::string title
                    , void *data )
                    : m_assetID( assetID )
                    , m_type( type )
                    , m_name( title )
                    , m_data( data )
{
}


AssetData::~AssetData()
{
    DeleteAllChildren();
}


void AssetData::AddChild( iAssetData *newChild )
{
    AssetDataPointerList::iterator it;
    iAssetData *iteratorData = NULL;

    for ( it = m_children.begin(); it != m_children.end(); it++ )
    {
        iteratorData = *it;

        if ( iteratorData->GetID() == newChild->GetID() ) 
        {
            throw( "Duplicate id" );
        }
    }

    m_children.push_back( newChild );
}


void AssetData::DeleteChild( AssetID id )
{
    AssetDataPointerList::iterator it;

    for ( it = m_children.begin(); it != m_children.end(); it++ )
    {
        if ( ( *it )->GetID() == id ) 
        {
            switch( ( *it )->GetType() )
            {
            case AssetItemType_filter:
                ( *it )->DeleteAllChildren();
                it = m_children.erase( it );
                break;

            case AssetItemType_testItem:
                it = m_children.erase( it );
                break;

            case AssetItemType_testGroup:
                //  Don't do anything yet...
                break;

            default:
                break;
            }

            break;
        }
    }
}


std::list<AssetID> AssetData::GetChildren()
{
    AssetIDList children;
    AssetDataPointerList::iterator it;

    for ( it = m_children.begin(); it != m_children.end(); it++ )
    {
        children.push_back( ( *it )->GetID() );
    }

    return children;
}


void AssetData::DeleteAllChildren()
{
    AssetDataPointerList::iterator it;

    for ( it = m_children.begin(); it != m_children.end(); )
    {
        switch( ( *it )->GetType() )
        {
        case AssetItemType_filter:
            ( *it )->DeleteAllChildren();
            delete *it;
            it = m_children.erase( it );
            break;

        case AssetItemType_testGroup:
            ( *it )->DeleteAllChildren();
            delete *it;
            it = m_children.erase( it );
            break;

        case AssetItemType_testItem:
            delete *it;
            it = m_children.erase( it );
            break;

        default:
            it++;
            break;
        }
    }
}
