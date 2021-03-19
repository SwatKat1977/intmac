//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef ASSETSTREECTRL_H
#define ASSETSTREECTRL_H

#include <map>
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <hci/mainDialog/assets/ToolbarItemData.h>
#include <dataModels/iAssetData.h>


class AssetsTreeCtrl : public wxTreeCtrl
{
public:
    AssetsTreeCtrl( wxWindow *parent
                  , wxWindowID id
                  , wxSize size );

    ~AssetsTreeCtrl();

    void AddAssetItem( AssetID itemID 
                     , AssetID parentItemID 
                     , std::string label
                     , AssetItemType type );

private:
    wxTreeItemId AddTreeItem( wxTreeItemId parent
                            , std::string label
                            , ToolbarItemSubType subType
                            , AssetID itemID
                            , int iconIndex );

    void OnTreeRightClick( wxTreeEvent &event );

    void OnAddTestingFilter( wxTreeEvent &event );

    void OnItemLabelEdit( wxTreeEvent &event );

private:
    wxImageList *m_treeIcons;
    wxWindowID m_treeCtrlID;
    wxTreeItemId m_treeRootID;
    wxTreeItemId m_currentSelection;
    std::map<AssetID, wxTreeItemId> m_wxtreeToAssetMap;
};

#endif
