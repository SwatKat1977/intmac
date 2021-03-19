//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef TOOLBARITEMDATA_H
#define TOOLBARITEMDATA_H

#include <wx/treectrl.h>


enum ToolbarItemSubType
{
    ToolbarItemSubType_none,
    ToolbarItemSubType_testingAssetsFilter,
    ToolbarItemSubType_testingAssetsTestItem,
    ToolbarItemSubType_testingAssetsTestGroup
};


class ToolbarItemData: public wxTreeItemData
{
public:

        ToolbarItemData( int id
                       , ToolbarItemSubType subType
                       , std::string name );

        ~ToolbarItemData();

        int GetItemID() { return m_itemID; }

        ToolbarItemSubType GetNodeSubType() { return m_subType; }

        std::string GetName() { return m_name; }

private:
    int m_itemID;
    ToolbarItemSubType m_subType;
    std::string m_name;
};


#endif
