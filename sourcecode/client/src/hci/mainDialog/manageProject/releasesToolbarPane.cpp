//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <hci/mainDialog/ManageProject/releasesToolbarPane.h>

const wxWindowID ID_releasesToolbarPane = 0x100;
const wxWindowID ID_treeCtrl = 0x101;


ReleasesToolbarPane::ReleasesToolbarPane( wxWindow *parent
                                         , const wxSize &size
                                         , iEventHandler *eventHandler )
                    : wxPanel( parent
                             , ID_releasesToolbarPane
                             , wxDefaultPosition
                             , size )
{
    wxBoxSizer *topLevelSizer = new wxBoxSizer( wxHORIZONTAL );

    m_treeCtrl = new ReleasesTreeCtrl( this
                                     , ID_treeCtrl
                                     , size
                                     , eventHandler );

    topLevelSizer->Add( m_treeCtrl
                      , wxEXPAND
                      , wxEXPAND );

    SetSizerAndFit( topLevelSizer );  
}


ReleasesToolbarPane::~ReleasesToolbarPane()
{
}


void ReleasesToolbarPane::AddItem( ReleaseTransaction *transaction )
{
    m_treeCtrl->AddTreeItem( transaction->GetID()
                           , transaction->GetParentID()
                           , transaction->GetTitle()
                           , transaction->GetItemType() );
}
