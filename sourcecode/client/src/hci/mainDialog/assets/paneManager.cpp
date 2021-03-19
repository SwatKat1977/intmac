//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <hci/mainDialog/assets/paneManager.h>
#include <wx/treectrl.h>


const int infoPanelMaxSize = 50;


paneManager::paneManager( wxWindow *managedWnd
                        , ToolbarPane *toolbarPane
                        , MainPane *mainPane )
            : wxAuiManager()
            , m_toolbarPane( toolbarPane )
            , m_mainPane( mainPane )
{
    SetManagedWindow( managedWnd );

    wxSize sizeInfo = m_toolbarPane->GetClientSize();

    AddPane( m_toolbarPane, wxLEFT );
    AddPane( m_mainPane, wxCENTER );
     
    //  ====================
    //  Update Toolbar panel
    //  ====================
    wxAuiPaneInfo &retrievedToolbarPane = GetPane( m_toolbarPane );
    retrievedToolbarPane.Floatable( false );
    retrievedToolbarPane.CaptionVisible( false );
    retrievedToolbarPane.BestSize( sizeInfo );

    Update();
}
 

paneManager::~paneManager()
{
}
