//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef COMMONINPUTBOX_H
#define COMMONINPUTBOX_H

#include <wx/dialog.h>

struct CommonInputBoxSettings
{
    CommonInputBoxSettings() : m_minimumLength( 0 )
                             , m_maximumLength( 0 )
                             , m_excludedCharacters( "" )
                             , m_tooltip( "" )
    {}

    unsigned int m_minimumLength;
    unsigned int m_maximumLength;
    std::string m_excludedCharacters;
    std::string m_tooltip;
};


class CommonInputBox : public wxDialog
{
public:
    CommonInputBox( wxWindow *parent
                  , const wxString &dialogTitle
                  , const wxString &inputLabel
                  , CommonInputBoxSettings *settings );

    ~CommonInputBox();

    std::string GetInputValue();

protected:
    wxStaticText *m_inputLabel;
    wxTextCtrl *m_inputTextBox;
    wxString m_inputString;
    CommonInputBoxSettings *m_settings;

protected:
    void OnCancel( wxCommandEvent &Event );    

    void OnOk( wxCommandEvent &event );
};

#endif
