//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <dataModels/ReleaseData.h>


ReleaseData::ReleaseData( ReleaseItemID ID
                        , ReleaseItemType type
                        , std::string title
                        , void *data )
           : m_ID( ID )
           , m_type( type )
           , m_name( title )
           , m_data( data )
{
}


ReleaseData::~ReleaseData()
{
    DeleteAllChildren();
}


void ReleaseData::AddChild( iReleaseData *newChild )
{
    ReleaseDataPointerList::iterator it;
    iReleaseData *iteratorData = NULL;

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


void ReleaseData::DeleteChild( ReleaseItemID id )
{
    ReleaseDataPointerList::iterator it;

    for ( it = m_children.begin(); it != m_children.end(); it++ )
    {
        if ( ( *it )->GetID() == id ) 
        {
            switch( ( *it )->GetType() )
            {
            case ReleaseItemType_release:
                ( *it )->DeleteAllChildren();
                it = m_children.erase( it );
                break;

            case ReleaseItemType_cycle:
                it = m_children.erase( it );
                break;

            case ReleaseItemType_folder:
                ( *it )->DeleteAllChildren();
                it = m_children.erase( it );
                break;

            default:
                break;
            }

            break;
        }
    }
}


ReleaseIDList ReleaseData::GetChildren()
{
    ReleaseIDList children;
    ReleaseDataPointerList::iterator it;

    for ( it = m_children.begin(); it != m_children.end(); it++ )
    {
        children.push_back( ( *it )->GetID() );
    }

    return children;
}


void ReleaseData::DeleteAllChildren()
{
    ReleaseDataPointerList::iterator it;

    for ( it = m_children.begin(); it != m_children.end(); )
    {
        switch( ( *it )->GetType() )
        {
        case ReleaseItemType_release:
            ( *it )->DeleteAllChildren();
            delete *it;
            it = m_children.erase( it );
            break;

        case ReleaseItemType_folder:
            ( *it )->DeleteAllChildren();
            delete *it;
            it = m_children.erase( it );
            break;

        case ReleaseItemType_cycle:
            delete *it;
            it = m_children.erase( it );
            break;

        default:
            it++;
            break;
        }
    }
}
