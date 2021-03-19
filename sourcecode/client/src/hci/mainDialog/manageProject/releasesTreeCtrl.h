//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef RELEASESTREECTRL_H
#define RELEASESTREECTRL_H

#include <map>
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <system/projectConstants.h>
#include <iEventHandler.h>


class ReleasesTreeCtrl : public wxTreeCtrl
{
public:
    ReleasesTreeCtrl( wxWindow *parent
                    , wxWindowID id
                    , wxSize size
                    , iEventHandler *eventHandler );

    ~ReleasesTreeCtrl();

    void AddTreeItem( ReleaseItemID itemID
                    , ReleaseItemID parent
                    , std::string label
                    , ReleaseItemType type );

private:
    wxWindowID m_treeCtrlID;
    wxTreeItemId m_rootItem;
    std::map<ReleaseItemID, wxTreeItemId> m_wxtreeToReleaseMap;
    wxTreeItemId m_currentSelection;
    iEventHandler *m_eventHandler;

    void OnRightClick( wxTreeEvent &event );

    void OnSelectItem( wxTreeEvent &event );
};

#endif
