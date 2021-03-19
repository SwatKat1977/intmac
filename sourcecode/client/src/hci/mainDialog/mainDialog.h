//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <wx/wx.h>
#include <dataModels/stateModel.h>
#include <dataModels/projectModel.h>
#include <hci/mainDialog/assets/assetsPage.h>
#include <hci/mainDialog/testPlan/testPlanPage.h>
#include <hci/mainDialog/manageProject/ManageProjectPage.h>
#include <iEventHandler.h>


enum CurrentPageSelected
{
    CurrentPageSelected_assets,
    CurrentPageSelected_testPlan,
    CurrentPageSelected_manageProject
};


class MainDialog : public wxFrame
{
public:
    MainDialog( ProjectModel *projectModel
              , StateModel *stateModel
              , iEventHandler *eventHandler );

    ~MainDialog();

private:
	wxToolBar *m_toolbar;
    wxTimer m_updateTimer;
    ProjectModel *m_projectModel;
    AssetsPage *m_assetsPage;
    TestPlanPage *m_testPlanPage;
    ManageProjectPage *m_manageProjectPage;
    CurrentPageSelected m_activePage;
    wxBoxSizer *m_sizer;

    void OnCloseApplication( wxCloseEvent &event );

    void OnTimerTick( wxTimerEvent &event );

    void OnToolbarProjectAssets( wxCommandEvent &WXUNUSED( event ) );

    void OnToolbarTestPlan( wxCommandEvent &WXUNUSED( event ) );

    void OnManageProject( wxCommandEvent &WXUNUSED( event ) );
};

#endif

