//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <hci/mainDialog/manageProject/releaseFolderDetailsPane.h>

const wxWindowID ID_releaseFolderDetailsPane = 0x100;


ReleaseFolderDetailsPane::ReleaseFolderDetailsPane( wxWindow *parent
                                                  , const wxSize &size )
                         : wxPanel( parent
                                  , ID_releaseFolderDetailsPane
                                  , wxDefaultPosition
                                  , size )
{
    wxBoxSizer* topSizer = new wxBoxSizer( wxVERTICAL );
    this->SetSizer(topSizer);
    
    ////////////////////////////////
    //  Name label and and input box
    ////////////////////////////////
    wxFlexGridSizer* controlGrid = new wxFlexGridSizer( 0, 2, 0, 0 );
    controlGrid->SetFlexibleDirection( wxBOTH );
    controlGrid->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    topSizer->Add( controlGrid, 1, wxALL | wxEXPAND, 5 );
    
    wxStaticText *labelName = new wxStaticText( this
                                              , wxID_ANY
                                              , _( "Name:" )
                                              , wxDefaultPosition
                                              , wxSize( -1, -1 )
                                              , wxALIGN_RIGHT );
    controlGrid->Add( labelName, 0, wxALL | wxALIGN_RIGHT, 5 );
    
    wxTextCtrl *txtName = new wxTextCtrl( this
                                        , wxID_ANY
                                        , wxT( "" )
                                        , wxDefaultPosition
                                        , wxSize( 200, -1 )
                                        , 0 );
    txtName->SetToolTip( _( "Required folder name" ) );
    controlGrid->Add(txtName, 0, wxALL, 5);
    

    ///////////////////////////////////////
    //  Description label and and input box
    ///////////////////////////////////////
    wxStaticText *labelDescription = new wxStaticText( this
                                                     , wxID_ANY
                                                     , _( "Description:" )
                                                     , wxDefaultPosition
                                                     , wxSize( -1, -1 )
                                                     , 0);
    controlGrid->Add( labelDescription, 0, wxALL, 5 );
    
    wxTextCtrl *txtDescription = new wxTextCtrl( this
                                               , wxID_ANY
                                               , wxT( "" )
                                               , wxDefaultPosition
                                               , wxSize( 200, 200 )
                                               , wxTE_NO_VSCROLL | wxTE_MULTILINE );
    txtDescription->SetToolTip( _( "Optional description of folder" ) );
    controlGrid->Add( txtDescription, 0, wxALL, 5 );
}


ReleaseFolderDetailsPane::~ReleaseFolderDetailsPane()
{
}
