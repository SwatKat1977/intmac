//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef MANAGEPROJECTPANEMANAGER_H
#define MANAGEPROJECTPANEMANAGER_H


#include <wx/wx.h>
#include <wx/aui/framemanager.h>
#include <hci/mainDialog/manageProject/releasesToolbarPane.h>
#include <hci/mainDialog/manageProject/manageProjectMain.h>


class ManageProjectPaneManager : public wxAuiManager
{
public:
    ManageProjectPaneManager( wxWindow *managedWnd
                            , ReleasesToolbarPane *toolbarPane
                            , ManageProjectMain *manageProjectMain );
 
    ~ManageProjectPaneManager();

private:
    ReleasesToolbarPane *m_toolbarPane;
    ManageProjectMain *m_manageProjectMain;
};

#endif
