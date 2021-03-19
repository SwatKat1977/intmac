//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <hci/mainDialog/manageProject/releasesTreeCtrl.h>
#include <hci/mainDialog/manageProject/ReleaseItemData.h>
#include <hci/mainDialog/manageProject/manageProjectMain.h>
#include <system/eventsList.h>


const wxString treeCtrlRootName = _T( "Releases" );
const std::string treeRootName = "Releases";


const int treeRootItem = 0x0;
const int toolbarIconIndex_root = -1;

std::string contextMenuItem_newReleaseFolder = "New Release Folder";
std::string contextMenuItem_newRelease = "New Release";
std::string contextMenuItem_newReleaseCycle = "New Cycle";

enum 
{
    MENUID_newReleaseFolder = wxID_HIGHEST + 1,
    MENUID_newRelease,
    MENUID_newReleaseCycle,
};


ReleasesTreeCtrl::ReleasesTreeCtrl( wxWindow *parent
                                  , wxWindowID id
                                  , wxSize size
                                  , iEventHandler *eventHandler )
                : wxTreeCtrl( parent
                            , id
                            , wxDefaultPosition
                            , size
                            , wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS )
                , m_treeCtrlID( id )
                , m_eventHandler( eventHandler )
{
    ReleaseItemData *rootData = new ReleaseItemData( treeRootItem
                                                   , treeRootName
                                                   , ReleaseItemType_folder );

    m_rootItem = AddRoot( treeCtrlRootName
                        , toolbarIconIndex_root
                        , toolbarIconIndex_root
                        , rootData );

    m_wxtreeToReleaseMap[treeRootItem] = m_rootItem;

    Connect( m_treeCtrlID
           , wxEVT_TREE_ITEM_RIGHT_CLICK
           , wxTreeEventHandler( ReleasesTreeCtrl::OnRightClick ) );

    Connect( m_treeCtrlID
           , wxEVT_TREE_SEL_CHANGED
           , wxTreeEventHandler( ReleasesTreeCtrl::OnSelectItem ) );  
}


ReleasesTreeCtrl::~ReleasesTreeCtrl()
{
}


void ReleasesTreeCtrl::AddTreeItem( ReleaseItemID itemID
                                  , ReleaseItemID parent
                                  , std::string label
                                  , ReleaseItemType type )
{
    wxTreeItemId parentTreeItem = m_wxtreeToReleaseMap[parent];
    int treeIconIndex = -1;
    ReleaseItemData *data = NULL;
    wxTreeItemId itemTreeID;

    data = new ReleaseItemData( itemID
                              , label
                              , type );

    itemTreeID = AppendItem( parentTreeItem
                           , label
                           , treeIconIndex
                           , treeIconIndex
                           , data );

    m_wxtreeToReleaseMap[itemID] = itemTreeID;
}


void ReleasesTreeCtrl::OnRightClick( wxTreeEvent &event )
{
   ReleaseItemData *itemData = NULL;
    wxMenu menu;

    //  Ensure that right-clicked item is in focus by forcably selecting it.
    SelectItem( event.GetItem() );

    m_currentSelection = GetSelection();

    if ( m_currentSelection.IsOk() )
    {
        itemData = static_cast<ReleaseItemData *>( GetItemData( m_currentSelection ) );

        switch( itemData->GetType() )
        {
        case ReleaseItemType_folder:
            menu.Append( MENUID_newReleaseFolder, contextMenuItem_newReleaseFolder );
            menu.Append( MENUID_newRelease, contextMenuItem_newRelease );
            break;

        case ReleaseItemType_release:
            menu.Append( MENUID_newReleaseCycle, contextMenuItem_newReleaseCycle );
            break;

        case ReleaseItemType_cycle:
            break;

        default:
            break;
        }

        PopupMenu( &menu );
    }
}


void ReleasesTreeCtrl::OnSelectItem( wxTreeEvent &event )
{
   ReleaseItemData *itemData = NULL;
    wxMenu menu;

    //  Ensure that right-clicked item is in focus by forcably selecting it.
    SelectItem( event.GetItem() );

    m_currentSelection = GetSelection();

    if ( m_currentSelection.IsOk() )
    {
        itemData = static_cast<ReleaseItemData *>( GetItemData( m_currentSelection ) );

        switch( itemData->GetType() )
        {
        case ReleaseItemType_folder:
            m_eventHandler->ReceiveMessage( Evt_Releases_ReleaseFolder );
            break;

        case ReleaseItemType_release:
            m_eventHandler->ReceiveMessage( Evt_Releases_Release );
            break;

        case ReleaseItemType_cycle:
            m_eventHandler->ReceiveMessage( Evt_Releases_ReleaseCycle );
            break;

        default:
            break;
        }
    }
}
