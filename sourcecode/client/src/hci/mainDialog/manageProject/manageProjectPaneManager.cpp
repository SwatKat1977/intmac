//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <hci/MainDialog/manageProject/manageProjectPaneManager.h>


const int infoPanelMaxSize = 150;


ManageProjectPaneManager::ManageProjectPaneManager( wxWindow *managedWnd
                                                  , ReleasesToolbarPane *toolbarPane
                                                  , ManageProjectMain *manageProjectMain )
            : wxAuiManager()
            , m_toolbarPane( toolbarPane )
            , m_manageProjectMain( manageProjectMain )
{
    SetManagedWindow( managedWnd );

    wxSize sizeInfo = m_toolbarPane->GetClientSize();

    AddPane( m_toolbarPane, wxLEFT );
    AddPane( m_manageProjectMain, wxCENTER );
     
    //  ====================
    //  Update Toolbar panel
    //  ====================
    wxAuiPaneInfo &retrievedToolbarPane = GetPane( m_toolbarPane );
    retrievedToolbarPane.Floatable( false );
    retrievedToolbarPane.CaptionVisible( false );
    retrievedToolbarPane.BestSize( sizeInfo );

    Update();
}


ManageProjectPaneManager::~ManageProjectPaneManager()
{
}
