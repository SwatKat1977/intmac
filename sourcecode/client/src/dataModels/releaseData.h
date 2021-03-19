//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef RELEASEDATA_H
#define RELEASEDATA_H


#include <dataModels/iReleaseData.h>

typedef std::list<iReleaseData *> ReleaseDataPointerList;


class ReleaseData : public iReleaseData
{
public:
    ReleaseData( ReleaseItemID ID
               , ReleaseItemType type
               , std::string title
               , void *data );

    ~ReleaseData();

    ReleaseItemID GetID() { return m_ID; }

    std::string GetName() { return m_name; }

    void *GetAssetData() { return m_data; }

    ReleaseItemType GetType() { return m_type; }

    void SetName( std::string name ) { m_name = name; }

    void SetData( void *data ) { m_data = data; }

    void AddChild( iReleaseData *newChild );

    void DeleteChild( ReleaseItemID id );

    ReleaseIDList GetChildren();

protected:
    ReleaseItemID m_ID;
    std::string m_name;
    void *m_data;
    ReleaseItemType m_type;
    ReleaseDataPointerList m_children;

    void DeleteAllChildren();
};

#endif
