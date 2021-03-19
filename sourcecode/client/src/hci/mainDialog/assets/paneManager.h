//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef PANEMANAGER_H
#define PANEMANAGER_H

#include <wx/wx.h>
#include <wx/aui/framemanager.h>
#include <hci/mainDialog/assets/toolbarPane.h>
#include <hci/mainDialog/assets/mainPane.h>


class paneManager : public wxAuiManager
{
public:
    paneManager( wxWindow *managedWnd
               , ToolbarPane *toolbarPane
               , MainPane *mainPane );
 
    ~paneManager();

private:
    ToolbarPane *m_toolbarPane;
    wxPanel *m_mainPane;
};

#endif
