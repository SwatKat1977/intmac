//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef PROJECTCONSTANTS_H
#define PROJECTCONSTANTS_H

#include <string>
#include <vector>

enum AssetItemType
{
    AssetItemType_filter,
    AssetItemType_testGroup,
    AssetItemType_testItem
};

enum ReleaseItemType
{
    ReleaseItemType_release,
    ReleaseItemType_folder,
    ReleaseItemType_cycle
};

typedef long ProjectID;
typedef long AssetItemID;
typedef long ReleaseItemID;

struct ProjectEntry
{
    ProjectID m_id;
    std::string m_name;
};

struct ProjectAssetEntry
{
    AssetItemID m_id;
    AssetItemID m_parentID;
    std::string m_name;
    AssetItemType m_type;
};

struct ReleaseEntry
{
    ReleaseItemID m_id;
    ReleaseItemID m_parentID;
    std::string m_name;
    ReleaseItemType m_type;
};

typedef std::vector<ProjectEntry> ProjectsList;
typedef std::vector<ProjectAssetEntry> ProjectAssetsList;
typedef std::vector<ReleaseEntry> ReleaseEntriesList;

#endif
