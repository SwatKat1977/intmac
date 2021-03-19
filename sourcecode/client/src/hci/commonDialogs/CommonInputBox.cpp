//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <wx/wx.h>
#include <hci/commonDialogs/CommonInputBox.h>

wxWindowID inputTextCtrlID = 9901;


CommonInputBox::CommonInputBox( wxWindow *parent
                              , const wxString &dialogTitle
                              , const wxString &inputLabel
                              , CommonInputBoxSettings *settings )
                              : wxDialog( parent
                                        , wxID_ANY
                                        , dialogTitle
                                        , wxDefaultPosition
                                        , wxSize( 300, 120 ) )
                              , m_settings( settings )
{
    wxGridSizer *sizer = NULL;
    wxPanel* panel = NULL;
    wxButton* cancelButton = NULL;
    wxButton* okButton = NULL;

    sizer = new wxGridSizer( 3, 5, 5 );

    panel = new wxPanel( this, wxID_ANY );

    m_inputLabel = new wxStaticText( panel 
                                   , wxID_ANY
                                   , inputLabel
                                   , wxDefaultPosition );

    wxTextValidator txtValidation( wxFILTER_EXCLUDE_CHAR_LIST
                                 , &m_inputString );
    
    if ( m_settings->m_excludedCharacters.size() )
    {
        txtValidation.SetCharExcludes( m_settings->m_excludedCharacters );
    }

    m_inputTextBox = new wxTextCtrl( panel
                                   , inputTextCtrlID
                                   , "" 
                                   , wxDefaultPosition
                                   , wxSize( 170, 20 )
                                   , 0L
                                   , txtValidation );

    if ( m_settings->m_maximumLength )
    {
        m_inputTextBox->SetMaxLength( m_settings->m_maximumLength );
    }

    m_inputTextBox->SetToolTip( m_settings->m_tooltip );

    okButton = new wxButton( panel, wxID_ANY, "OK" );
    cancelButton = new wxButton( panel, wxID_ANY, "Cancel" );

    sizer->Add( m_inputLabel
              , 0
              , wxALL
              , 15 );
    sizer->Add( m_inputTextBox
              , 0
              , wxALL
              , 15 );
    sizer->AddStretchSpacer(1);

    sizer->AddStretchSpacer(1);
    sizer->Add( okButton, 0, 
                wxALL | wxALIGN_RIGHT | wxALIGN_BOTTOM, 10);
    sizer->Add( cancelButton, 0, 
                wxALL | wxALIGN_RIGHT | wxALIGN_BOTTOM, 10);

    panel->SetSizer( sizer );

    Bind( wxEVT_COMMAND_BUTTON_CLICKED
        , &CommonInputBox::OnCancel
        , this
        , cancelButton->GetId() );
    Bind( wxEVT_COMMAND_BUTTON_CLICKED
        , &CommonInputBox::OnOk
        , this
        , okButton->GetId() );
    Center();
}


CommonInputBox::~CommonInputBox()
{
}


void CommonInputBox::OnCancel( wxCommandEvent &WXUNUSED( Event ) ) 
{
    EndModal( wxID_CANCEL );
    Destroy();
}


void CommonInputBox::OnOk( wxCommandEvent &WXUNUSED( event ) )    
{
    std::string inputString = GetInputValue();

    if ( inputString.size() < m_settings->m_minimumLength )
    {
        wxString errmsg = m_settings->m_tooltip;
        wxMessageDialog errorDialog( NULL
                                   , errmsg
                                   , wxT( "Validation Error" )
                                   ,wxOK );
        errorDialog.ShowModal();
    }
    else
    {
        EndModal( wxID_OK );
        Destroy();
    }
}


std::string CommonInputBox::GetInputValue()    
{ 
    return m_inputTextBox->GetValue().ToStdString();    
}
