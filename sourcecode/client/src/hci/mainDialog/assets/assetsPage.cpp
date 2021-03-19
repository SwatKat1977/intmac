//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <hci/MainDialog/assets/assetsPage.h>
#include <hci/MainDialog/assets/toolBarPane.h>
#include <hci/mainDialog/assets/mainPane.h>

const int toolbarPanelDefaultWidth = 150; 
const long style = wxTAB_TRAVERSAL | wxNO_BORDER;


AssetsPage::AssetsPage( wxWindow *parent,
                        wxSize &size,
                        ProjectModel *projectModel )
           : wxPanel( parent
                    , wxID_ANY
                    , wxDefaultPosition
                    , size
                    , style )
           , m_projectModel( projectModel )
{
    wxSize dialogClientSize = GetClientSize();

    m_toolbarPane = new ToolbarPane( this
                                   , wxSize( toolbarPanelDefaultWidth  
                                           , dialogClientSize.GetHeight() ) );
    m_mainPane = new MainPane( this );

    m_paneManager = new paneManager( this
                                   , m_toolbarPane
                                   , m_mainPane );
}


AssetsPage::~AssetsPage()
{
    m_paneManager->UnInit();

    if ( m_toolbarPane ) delete m_toolbarPane;
    if ( m_mainPane ) delete m_mainPane;
    if ( m_paneManager ) delete m_paneManager;
}


void AssetsPage::UpdatePage()
{
    AssetDataTransaction *transaction = NULL;
    int transactionsWaiting = m_projectModel->GetNumberOfAssetTransactions();

    if ( transactionsWaiting > 0 )
    {
        for ( int transNo = 0; transNo < transactionsWaiting; transNo++ )
        {
            transaction = m_projectModel->GetTopAssetTransaction();

            switch( transaction->GetType() )
            {
            case AssetDataTransactionType_add:
                m_toolbarPane->AddAssetItem( transaction );
                m_projectModel->PopTopAssetTransaction();
                break;

            default:
                break;
            }
        }
    }
}


void AssetsPage::Refresh()
{
    m_toolbarPane->Refresh();
}
