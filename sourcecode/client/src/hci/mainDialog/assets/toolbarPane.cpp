//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <hci/mainDialog/assets/toolbarPane.h>


wxWindowID ID_treeCtrl = ID_toolbarPane + 0x01;


ToolbarPane::ToolbarPane( wxWindow *parent
                        , const wxSize &size )
            : wxPanel( parent
                     , ID_toolbarPane
                     , wxDefaultPosition
                     , size )
{
    wxBoxSizer *topLevelSizer = new wxBoxSizer( wxHORIZONTAL );

    m_treeCtrl = new AssetsTreeCtrl( this
                                   , ID_treeCtrl
                                   , size );

    topLevelSizer->Add( m_treeCtrl
                      , wxEXPAND
                      , wxEXPAND );

    SetSizerAndFit( topLevelSizer );  
}


ToolbarPane::~ToolbarPane()
{
    if ( m_treeCtrl ) delete m_treeCtrl;
}


void ToolbarPane::AddAssetItem( AssetDataTransaction *transaction )
{
    m_treeCtrl->AddAssetItem( transaction->GetID()
                            , transaction->GetParentID()
                            , transaction->GetTitle()
                            , transaction->GetAssetData()->GetType() );
}
