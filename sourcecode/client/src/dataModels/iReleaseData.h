//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IRELEASEDATA_H
#define IRELEASEDATA_H

#include <string>
#include <list>
#include <system/projectConstants.h>

//typedef unsigned long AssetID;
typedef std::list<ReleaseItemID> ReleaseIDList;


class iReleaseData
{
public:
    virtual ~iReleaseData() {}

    virtual ReleaseItemID GetID() = 0;

    virtual std::string GetName() = 0;

    virtual void *GetAssetData() = 0;

    virtual ReleaseItemType GetType() = 0;

    virtual void SetName( std::string name ) = 0;

    virtual void SetData( void *data ) = 0;

    virtual void AddChild( iReleaseData *newChild ) = 0;

    virtual void DeleteChild( ReleaseItemID id ) = 0;

    virtual ReleaseIDList GetChildren() = 0;

    virtual void DeleteAllChildren() = 0;
};

#endif
