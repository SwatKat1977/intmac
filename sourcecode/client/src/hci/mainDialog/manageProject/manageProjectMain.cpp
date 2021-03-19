//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <hci/mainDialog/manageProject/manageProjectMain.h>
#include <system/eventsList.h>


ManageProjectMain::ManageProjectMain( wxWindow *parent, iEventHandler *eventHandler )
                         : wxPanel( parent
                                  , wxID_ANY
                                  , wxDefaultPosition )
{
    wxSize dialogClientSize = GetClientSize();

    m_activePage = ProjectMainSubPage_folderDetails;

    m_sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(m_sizer);

    //  Release Folder Details Pane
    m_releaseFolderDetailsPane = new ReleaseFolderDetailsPane( this
                                                             , dialogClientSize);
    m_sizer->Add( m_releaseFolderDetailsPane, 1, wxGROW);

    //  Release Cycle Details Pane
    m_releaseCycleDetailsPane = new ReleaseCycleDetailsPane( this
                                                           , dialogClientSize);
    m_releaseCycleDetailsPane->Hide();

    //  Release Details Pane
    m_releaseDetailsPane = new ReleaseDetailsPane( this
                                                 , dialogClientSize );
    m_releaseDetailsPane->Hide();

    eventHandler->RegisterObserver( this );
}


ManageProjectMain::~ManageProjectMain()
{
}


void ManageProjectMain::SelectSubPageFolderDetails()
{
    switch( m_activePage )
    {
    //  This is already selected.. ignore it!
    case ProjectMainSubPage_folderDetails:
        break;

    case ProjectMainSubPage_cycleDetails:
        m_sizer->Detach( 0 ); 
        m_releaseCycleDetailsPane->Hide();
        m_sizer->Prepend( m_releaseFolderDetailsPane, 1, wxGROW );
        m_releaseFolderDetailsPane->Show();
        break;

    case ProjectMainSubPage_releaseDetails:
        m_sizer->Detach( 0 ); 
        m_releaseDetailsPane->Hide();
        m_sizer->Prepend( m_releaseFolderDetailsPane, 1, wxGROW );
        m_releaseFolderDetailsPane->Show();
        break;
    }

    m_activePage = ProjectMainSubPage_folderDetails;
    m_sizer->Layout();
    m_releaseFolderDetailsPane->Refresh();
}


void ManageProjectMain::SelectSubPageCycleDetails()
{
    switch( m_activePage )
    {
    //  This is already selected.. ignore it!
    case ProjectMainSubPage_cycleDetails:
        break;

    case ProjectMainSubPage_folderDetails:
        m_sizer->Detach( 0 ); 
        m_releaseFolderDetailsPane->Hide();
        m_sizer->Prepend( m_releaseCycleDetailsPane, 1, wxGROW );
        m_releaseCycleDetailsPane->Show();
        break;

    case ProjectMainSubPage_releaseDetails:
        m_sizer->Detach( 0 ); 
        m_releaseDetailsPane->Hide();
        m_sizer->Prepend( m_releaseCycleDetailsPane, 1, wxGROW );
        m_releaseCycleDetailsPane->Show();
        break;
    }

    m_activePage = ProjectMainSubPage_cycleDetails;
    m_sizer->Layout();
    m_releaseCycleDetailsPane->Refresh();
}


void ManageProjectMain::SelectSubPageReleaseDetails()
{
    switch( m_activePage )
    {
    //  This is already selected.. ignore it!
    case ProjectMainSubPage_releaseDetails:
        break;

    case ProjectMainSubPage_cycleDetails:
        m_sizer->Detach( 0 ); 
        m_releaseCycleDetailsPane->Hide();
        m_sizer->Prepend( m_releaseDetailsPane, 1, wxGROW );
        m_releaseDetailsPane->Show();
        break;

    case ProjectMainSubPage_folderDetails:
        m_sizer->Detach( 0 ); 
        m_releaseFolderDetailsPane->Hide();
        m_sizer->Prepend( m_releaseDetailsPane, 1, wxGROW );
        m_releaseDetailsPane->Show();
        break;
    }

    m_activePage = ProjectMainSubPage_releaseDetails;
    m_sizer->Layout();
    m_releaseDetailsPane->Refresh();
}


void ManageProjectMain::ReceiveEventMessage( EventID ID
                                           , iEventMsg *msg )
{
    switch( ID )
    {
    case Evt_Releases_Release:
        SelectSubPageReleaseDetails();
        break;

    case Evt_Releases_ReleaseFolder:
        SelectSubPageFolderDetails();
        break;

    case Evt_Releases_ReleaseCycle:
        SelectSubPageCycleDetails();
        break;

    default:
        break;
    }
}
