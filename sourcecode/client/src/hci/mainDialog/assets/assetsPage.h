//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef ASSETSPAGE_H
#define ASSETSPAGE_H

#include <wx/wx.h>
#include <hci/mainDialog/assets/paneManager.h>
#include <dataModels/ProjectModel.h>


class AssetsPage : public wxPanel
{
public:
    AssetsPage( wxWindow *parent,
                wxSize &size,
                ProjectModel *projectModel );

    ~AssetsPage();

    void Refresh();

    void UpdatePage();

private:
    paneManager *m_paneManager;
    ToolbarPane *m_toolbarPane;
    MainPane *m_mainPane;
    ProjectModel *m_projectModel;
};

#endif
