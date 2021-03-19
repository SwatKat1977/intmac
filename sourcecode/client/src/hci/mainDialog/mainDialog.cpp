//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <hci/MainDialog/MainDialog.h>


wxWindowID ID_updateGUITimer = 0x100;

wxWindowID ID_toolbarProjectAssets = 0x110;
wxWindowID ID_toolbarTestPlan = 0x120;
wxWindowID ID_toolbarManageProject = 0x130;


const int dialogWidth  = 800; 
const int dialogHeight = 600;
const long dialogStyle = wxCLOSE_BOX | wxCAPTION | wxSYSTEM_MENU | wxRESIZE_BORDER;

wxString defaultDialogName = _T( "Integrated Test Manager Client" );


MainDialog::MainDialog( ProjectModel *projectModel
                      , StateModel *stateModel
                      , iEventHandler *eventHandler )
               : wxFrame( NULL
                        , wxID_ANY
                        , defaultDialogName
                        , wxDefaultPosition
                        , wxSize( dialogWidth, dialogHeight )
                        , dialogStyle )
               , m_projectModel( projectModel )
{
    wxSize dialogClientSize = GetClientSize();
    wxStatusBar *statusBar = NULL;

    m_sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(m_sizer);

    //  Assets Panel/Page
    m_assetsPage = new AssetsPage( this
                                 , dialogClientSize
                                 , projectModel );
    m_sizer->Add( m_assetsPage, 1, wxGROW);

    //  Test Plan Panel/Page
    m_testPlanPage = new TestPlanPage( this
                                     , wxID_ANY
                                     , wxDefaultPosition
                                     , wxDefaultSize
                                     , wxTAB_TRAVERSAL);
    m_testPlanPage->Hide();

    //  Manage Project Panel/Page
    m_manageProjectPage = new ManageProjectPage( this
                                               , dialogClientSize
                                               , projectModel
                                               , eventHandler );
    m_manageProjectPage->Hide();

    m_activePage = CurrentPageSelected_assets;

    //  -------------------
    //  Status Bar creation
    //  -------------------
    statusBar = new wxStatusBar( this, wxID_ANY );
    statusBar->SetFieldsCount( 2 );
 
    SetStatusBar( statusBar );

    std::string statusUserStr = "User: ";
    statusUserStr.append( stateModel->GetRealm() );
    statusUserStr.append( "\\" );
    statusUserStr.append( stateModel->GetUser() );
    statusBar->SetStatusText( statusUserStr, 0 );

    std::string statusProjectStr = "Project: ";
    statusProjectStr.append( stateModel->GetSelectedProjectName() );
    statusBar->SetStatusText( statusProjectStr, 1 );

    //  ----------------
    //  Toolbar creation
    //  ----------------
    m_toolbar = CreateToolBar();
    m_toolbar->SetToolBitmapSize( wxSize( 24, 24 ) );

    wxInitAllImageHandlers();

    wxBitmap manageProjectIcon;
    manageProjectIcon.LoadFile( _( "media/Apps-gnome-control-center-icon.png" )
                              , ( wxBITMAP_TYPE_PNG ) );
    m_toolbar->AddTool( ID_toolbarManageProject
                      , ""
                      , manageProjectIcon
                      , "Manage Project" );

    wxBitmap projectAssetsIcon;
    projectAssetsIcon.LoadFile( _( "media/projectAssets.png" )
                              , ( wxBITMAP_TYPE_PNG ) );
    m_toolbar->AddTool( ID_toolbarProjectAssets
                      , ""
                      , projectAssetsIcon
                      , "Project Assets" );

    wxBitmap testPlanIcon;
    testPlanIcon.LoadFile( _( "media/Apps-system-software-update-icon.png" )
                         , ( wxBITMAP_TYPE_PNG ) );
    m_toolbar->AddTool( ID_toolbarTestPlan
                      , ""
                      , testPlanIcon
                      , "Test Plan" );

    m_toolbar->Realize();

    Sleep(400);
    wxTimerEvent unused;
    OnTimerTick( unused );

    m_updateTimer.SetOwner( this, ID_updateGUITimer );
    m_updateTimer.Start( 2000, wxTIMER_CONTINUOUS );    

    Connect( ID_updateGUITimer
           , wxEVT_TIMER
           , wxTimerEventHandler( MainDialog::OnTimerTick ) );

    Connect( GetId()
           , wxEVT_CLOSE_WINDOW
           , wxCloseEventHandler( MainDialog::OnCloseApplication ) );

    Connect( ID_toolbarManageProject
           , wxEVT_COMMAND_MENU_SELECTED
           , wxCommandEventHandler( MainDialog::OnManageProject ) );

    Connect( ID_toolbarProjectAssets
           , wxEVT_COMMAND_MENU_SELECTED
           , wxCommandEventHandler( MainDialog::OnToolbarProjectAssets ) );

    Connect( ID_toolbarTestPlan
           , wxEVT_COMMAND_MENU_SELECTED
           , wxCommandEventHandler( MainDialog::OnToolbarTestPlan ) );
}


MainDialog::~MainDialog()
{
}


void MainDialog::OnCloseApplication( wxCloseEvent &event )
{
    m_updateTimer.Stop();

    Destroy();
}


void MainDialog::OnTimerTick( wxTimerEvent &event )
{
    m_assetsPage->UpdatePage();
    m_manageProjectPage->UpdatePage();
}


void MainDialog::OnToolbarProjectAssets( wxCommandEvent&WXUNUSED( event ) )
{
    switch( m_activePage )
    {
    //  This is already selected.. ignore it!
    case CurrentPageSelected_assets:
        break;

    case CurrentPageSelected_testPlan:
        m_sizer->Detach( 0 ); 
        m_testPlanPage->Hide();
        m_sizer->Prepend( m_assetsPage, 1, wxGROW );
        m_assetsPage->Show();
        break;

    case CurrentPageSelected_manageProject:
        m_sizer->Detach( 0 ); 
        m_manageProjectPage->Hide();
        m_sizer->Prepend( m_assetsPage, 1, wxGROW );
        m_assetsPage->Show();
        break;
    }

    m_activePage = CurrentPageSelected_assets;
    m_sizer->Layout();
    m_assetsPage->Refresh();
}


void MainDialog::OnToolbarTestPlan( wxCommandEvent&WXUNUSED( event ) )
{
    switch( m_activePage )
    {
    case CurrentPageSelected_assets:
        m_sizer->Detach( 0 ); 
        m_assetsPage->Hide();
        m_sizer->Prepend( m_testPlanPage, 1, wxGROW );
        m_testPlanPage->Show();
        m_sizer->Layout();
        break;

    //  This is already selected.. ignore it!
    case CurrentPageSelected_testPlan:
        break;

    case CurrentPageSelected_manageProject:
        m_sizer->Detach( 0 ); 
        m_manageProjectPage->Hide();
        m_sizer->Prepend( m_testPlanPage, 1, wxGROW );
        m_testPlanPage->Show();
        m_sizer->Layout();
        break;
    }

    m_activePage = CurrentPageSelected_testPlan;
}


void MainDialog::OnManageProject( wxCommandEvent &WXUNUSED( event ) )
{
    switch( m_activePage )
    {
    case CurrentPageSelected_assets:
        m_sizer->Detach( 0 ); 
        m_assetsPage->Hide();
        m_sizer->Prepend( m_manageProjectPage, 1, wxGROW );
        m_manageProjectPage->Show();
        break;

    case CurrentPageSelected_testPlan:
        m_sizer->Detach( 0 ); 
        m_testPlanPage->Hide();
        m_sizer->Prepend( m_manageProjectPage, 1, wxGROW );
        m_manageProjectPage->Show();
        break;

    //  This is already selected.. ignore it!
    case CurrentPageSelected_manageProject:
        break;
    }

    m_activePage = CurrentPageSelected_manageProject;
    m_sizer->Layout();
    m_manageProjectPage->Refresh();
}
