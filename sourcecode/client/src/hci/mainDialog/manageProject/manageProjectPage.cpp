//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <hci/MainDialog/manageProject/manageProjectPage.h>
#include <dataModels/releaseTransaction.h>


const long style = wxTAB_TRAVERSAL;
const int toolbarPanelDefaultWidth = 150; 


ManageProjectPage::ManageProjectPage( wxWindow *parent
                                    , const wxSize &size
                                    , ProjectModel *projectModel
                                    , iEventHandler *eventHandler )
                  : wxPanel( parent
                           , wxID_ANY
                           , wxDefaultPosition
                           , size
                           , style )
                  , m_projectModel( projectModel )
{
    wxSize dialogClientSize = GetClientSize();


    m_toolbarPane = new ReleasesToolbarPane( this
                                           , wxSize( toolbarPanelDefaultWidth  
                                           , dialogClientSize.GetHeight() )
                                           , eventHandler );

    m_manageProjectMain = new ManageProjectMain( this
                                               , eventHandler );

    m_paneManager = new ManageProjectPaneManager( this
                                                , m_toolbarPane
                                                , m_manageProjectMain );
}


ManageProjectPage::~ManageProjectPage()
{
    m_paneManager->UnInit();

    if ( m_toolbarPane ) delete m_toolbarPane;
    if ( m_manageProjectMain ) delete m_manageProjectMain;
    if ( m_paneManager ) delete m_paneManager;
}


void ManageProjectPage::Refresh()
{
    m_toolbarPane->Refresh();
}


void ManageProjectPage::UpdatePage()
{
    ReleaseTransaction *transaction = NULL;
    int transactionsWaiting = m_projectModel->GetNumberOfReleaseTransactions();

    while( m_projectModel->GetNumberOfReleaseTransactions() != 0 )
    {
        transaction = m_projectModel->GetTopReleaseTransaction();

        switch( transaction->GetTransactionType() )
        {
        case ReleaseTransactionType_add:
            m_toolbarPane->AddItem( transaction );
            m_projectModel->PopTopReleaseTransaction();
            break;

        default:
            break;
        }
    }
}
