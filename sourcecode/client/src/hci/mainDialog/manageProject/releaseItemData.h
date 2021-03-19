//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef RELEASEITEMDATA_H
#define RELEASEITEMDATA_H

#include <wx/treectrl.h>
#include <system/projectConstants.h>


class ReleaseItemData: public wxTreeItemData
{
public:
        ReleaseItemData( int id
                       , std::string name
                       , ReleaseItemType type );

        ~ReleaseItemData();

        int GetItemID() { return m_itemID; }

        std::string GetName() { return m_name; }

        ReleaseItemType GetType() { return m_type; }

private:
    int m_itemID;
    std::string m_name;
    ReleaseItemType m_type;
};


#endif
