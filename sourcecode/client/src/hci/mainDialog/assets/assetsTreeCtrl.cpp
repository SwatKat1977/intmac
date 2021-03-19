//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <hci/mainDialog/assets/assetsTreeCtrl.h>
#include <hci/mainDialog/assets/ToolbarItemData.h>
#include <hci/commonDialogs/CommonInputBox.h>


const wxString treeCtrlRootName = _T( "Project Assets" );
const std::string treelRootName = "Project Assets";

const int treeRootItem = 0x0;


std::string contextMenuItem_addFilter = "Add Filter";
std::string contextMenuItem_deleteFilter = "Delete Filter";
std::string contextMenuItem_addTestItem = "Add Test Item";
std::string contextMenuItem_deleteTestItem = "Delete Test Item";
std::string contextMenuItem_addTestGroup = "Add Test Group";
std::string contextMenuItem_deleteTestGroup = "Delete Test Group";


enum 
{
    MENUID_addFilter = wxID_HIGHEST + 1,
    MENUID_deleteFilter,
    MENUID_addTestItem,
    MENUID_deleteTestItem,
    MENUID_addTestGroup,
    MENUID_deleteTestGroup
};


enum toolbarIconIndex
{
    toolbarIconIndex_filter = 0,
    toolbarIconIndex_test,
    toolbarIconIndex_testGroup,
    toolbarIconIndex_root
};


AssetsTreeCtrl::AssetsTreeCtrl( wxWindow *parent
                              , wxWindowID id
                              , wxSize size )
              : wxTreeCtrl( parent
                          , id
                          , wxDefaultPosition
                          , size
                          , wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS )
              , m_treeCtrlID( id )
{
    ToolbarItemData *rootData = NULL;

    wxInitAllImageHandlers();

    m_treeIcons = new wxImageList( 24, 24 );

    wxBitmap filterIcon;
    filterIcon.LoadFile( _("media/Folder-documents-icon.png" )
                       , (wxBITMAP_TYPE_PNG) );

    wxBitmap testIcon;
    testIcon.LoadFile( _("media/Apps-default-applications-icon.png" )
                     , (wxBITMAP_TYPE_PNG) );

    wxBitmap testGroupIcon;
    testGroupIcon.LoadFile( _("media/Apps-file-manager-icon.png" )
                          , (wxBITMAP_TYPE_PNG) );

    wxBitmap projectAssetsIcon;
    projectAssetsIcon.LoadFile( _( "media/projectAssets.png" )
                              , ( wxBITMAP_TYPE_PNG ) );

    m_treeIcons->Add( filterIcon );
    m_treeIcons->Add( testIcon );
    m_treeIcons->Add( testGroupIcon );
    m_treeIcons->Add( projectAssetsIcon );

    rootData = new ToolbarItemData( treeRootItem
                                  , ToolbarItemSubType_none
                                  , treelRootName );

    m_treeRootID = AddRoot( treeCtrlRootName
                          , toolbarIconIndex_root
                          , toolbarIconIndex_root
                          , rootData );

    AssignImageList( m_treeIcons);

    m_wxtreeToAssetMap[treeRootItem] = m_treeRootID;

    Connect( m_treeCtrlID
           , wxEVT_TREE_ITEM_RIGHT_CLICK
           , wxTreeEventHandler( AssetsTreeCtrl::OnTreeRightClick ) );

    Connect( m_treeCtrlID
           , wxEVT_TREE_BEGIN_LABEL_EDIT
           , wxTreeEventHandler( AssetsTreeCtrl::OnItemLabelEdit ) );

    Connect( MENUID_addFilter
           , wxEVT_COMMAND_MENU_SELECTED
           , wxTreeEventHandler( AssetsTreeCtrl::OnAddTestingFilter ) );
}


AssetsTreeCtrl::~AssetsTreeCtrl()
{
}


void AssetsTreeCtrl::OnTreeRightClick( wxTreeEvent &event )
{
    ToolbarItemData *itemData = NULL;
    wxMenu menu;

    //  Ensure that right-clicked item is in focus by forcably selecting it.
    SelectItem( event.GetItem() );

    m_currentSelection = GetSelection();

    if ( m_currentSelection.IsOk() )
    {
        itemData = static_cast<ToolbarItemData *>( GetItemData( m_currentSelection ) );

        //  Don't create menu on top-level
        if ( itemData->GetItemID() == treeRootItem ) return;
    
        ToolbarItemSubType subType = itemData->GetNodeSubType();

        switch( itemData->GetNodeSubType() )
        {
        case ToolbarItemSubType_testingAssetsFilter:
            menu.Append( MENUID_deleteFilter, contextMenuItem_deleteFilter );
            menu.Append( MENUID_addFilter, contextMenuItem_addFilter );
            menu.AppendSeparator();
            menu.Append( MENUID_addTestItem, contextMenuItem_addTestItem );
            menu.AppendSeparator();
            menu.Append( MENUID_addTestGroup, contextMenuItem_addTestGroup );
            break;

        case ToolbarItemSubType_testingAssetsTestItem:
            menu.Append( MENUID_deleteTestItem, contextMenuItem_deleteTestItem );
            break;

        case ToolbarItemSubType_testingAssetsTestGroup:
            menu.Append( MENUID_deleteTestGroup, contextMenuItem_deleteTestGroup );
            menu.Append( MENUID_addTestItem, contextMenuItem_addTestItem );
            break;

        default:
            break;
        }

        PopupMenu( &menu );
    }
}


wxTreeItemId AssetsTreeCtrl::AddTreeItem( wxTreeItemId parent
                                        , std::string label
                                        , ToolbarItemSubType subType
                                        , AssetID itemID
                                        , int iconIndex )
{
    ToolbarItemData *data = NULL;
    wxTreeItemId itemTreeID;

    data = new ToolbarItemData( itemID
                              , subType
                              , label );

    itemTreeID = AppendItem( parent
                           , label
                           , iconIndex
                           , iconIndex
                           , data );

    return itemTreeID;
}


void AssetsTreeCtrl::OnAddTestingFilter( wxTreeEvent &event )
{
    ToolbarItemData *itemData = NULL;

    if ( m_currentSelection.IsOk() == false ) return;

    itemData = static_cast<ToolbarItemData *>( GetItemData( m_currentSelection ) );

    if ( m_currentSelection.IsOk() )
    {
        CommonInputBox *filterInput = NULL;
        CommonInputBoxSettings settings;

        settings.m_tooltip = "Alpha numeric or numbers only permitted.\
                              \nMinimum of 1 character";
        settings.m_excludedCharacters = "!@#$%^&*={}~§";
        settings.m_minimumLength = 1;

        filterInput = new CommonInputBox( NULL
                                        , _T( "Enter new filter name" )
                                        , _T( "Filter Name:" )
                                        , &settings );

        if ( filterInput->ShowModal() == wxID_OK )
        {
            AddTreeItem( m_currentSelection
                       , filterInput->GetInputValue()
                       , itemData->GetNodeSubType()
                       , 10090
                       , -1 );
        }

        delete filterInput;
    }
}


void AssetsTreeCtrl::OnItemLabelEdit( wxTreeEvent &event )
{
    ToolbarItemData *itemData = NULL;

    //  Ensure that right-clicked item is in focus by forcably selecting it.
    SelectItem( event.GetItem() );

    m_currentSelection = GetSelection();

    if ( m_currentSelection.IsOk() == false ) return;

    itemData = static_cast<ToolbarItemData *>( GetItemData( m_currentSelection ) );

    if ( itemData->GetId() == m_treeRootID ) event.Veto();
}


void AssetsTreeCtrl::AddAssetItem( AssetID itemID 
                                 , AssetID parentItemID 
                                 , std::string label
                                 , AssetItemType type )
{

    wxTreeItemId parentTreeItem = m_wxtreeToAssetMap[parentItemID];
    int treeIconIndex = -1;
    ToolbarItemSubType subType;

    switch( type )
    {
    case AssetItemType_filter:
    {
        treeIconIndex = toolbarIconIndex_filter;
        subType = ToolbarItemSubType_testingAssetsFilter;
    }
        break;

    case AssetItemType_testItem:
    {
        treeIconIndex = toolbarIconIndex_test;
        subType = ToolbarItemSubType_testingAssetsTestItem;
    }
        break;

    case AssetItemType_testGroup:
    {
        treeIconIndex = toolbarIconIndex_testGroup;
        subType = ToolbarItemSubType_testingAssetsTestGroup;
    }
        break;

    default:
        subType = ToolbarItemSubType_testingAssetsFilter;
        treeIconIndex = -1;
    }

    wxTreeItemId newTreeItem = AddTreeItem( parentTreeItem
                                          , label
                                          , subType
                                          , itemID
                                          , treeIconIndex );

    m_wxtreeToAssetMap[itemID] = newTreeItem;
}
