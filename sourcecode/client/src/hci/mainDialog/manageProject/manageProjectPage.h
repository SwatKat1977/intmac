//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef MANAGEPROJECTPANE_H
#define MANAGEPROJECTPANE_H

#include <wx/wx.h>
#include <hci/mainDialog/ManageProject/ManageProjectPaneManager.h>
#include <hci/mainDialog/ManageProject/ManageProjectMain.h>
#include <dataModels/ProjectModel.h>
#include <iEventHandler.h>


class ManageProjectPage : public wxPanel
{
public:
    ManageProjectPage( wxWindow *parent
                     , const wxSize &size
                     , ProjectModel *projectModel
                     , iEventHandler *eventHandler );

    ~ManageProjectPage();

    void Refresh();

    void UpdatePage();

private:
    ManageProjectPaneManager *m_paneManager;
    ReleasesToolbarPane *m_toolbarPane;
    ProjectModel *m_projectModel;
    ManageProjectMain *m_manageProjectMain;
};

#endif
