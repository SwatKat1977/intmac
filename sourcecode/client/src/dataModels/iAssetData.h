//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IASSETDATA_H
#define IASSETDATA_H

#include <string>
#include <list>
#include <system/projectConstants.h>

typedef unsigned long AssetID;
typedef std::list<AssetID> AssetIDList;


class iAssetData
{
public:
    virtual ~iAssetData() {}

    virtual AssetID GetID() = 0;

    virtual std::string GetName() = 0;

    virtual void *GetAssetData() = 0;

    virtual AssetItemType GetType() = 0;

    virtual void SetName( std::string name ) = 0;

    virtual void SetData( void *data ) = 0;

    virtual void AddChild( iAssetData *newChild ) = 0;

    virtual void DeleteChild( AssetID id ) = 0;

    virtual AssetIDList GetChildren() = 0;

    virtual void DeleteAllChildren() = 0;
};

#endif
