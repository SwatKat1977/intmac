//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef MANAGEPROJECTMAIN_H
#define MANAGEPROJECTMAIN_H

#include <wx/wx.h>
#include <hci/mainDialog/manageProject/releaseFolderDetailsPane.h>
#include <hci/mainDialog/manageProject/releaseCycleDetailsPane.h>
#include <hci/mainDialog/manageProject/releaseDetailsPane.h>
#include <iEventEnabledObject.h>
#include <iEventHandler.h>


enum ProjectMainSubPage 
{
    ProjectMainSubPage_folderDetails,
    ProjectMainSubPage_cycleDetails,
    ProjectMainSubPage_releaseDetails
};


class ManageProjectMain : public wxPanel, public iEventEnabledObject
{
public:
    ManageProjectMain( wxWindow *parent, iEventHandler *eventHandler );

    ~ManageProjectMain();

    void SelectSubPageFolderDetails();
    void SelectSubPageCycleDetails();
    void SelectSubPageReleaseDetails();

    void ReceiveEventMessage( EventID ID
                            , iEventMsg *msg );

private:
    wxBoxSizer *m_sizer;
    ReleaseFolderDetailsPane *m_releaseFolderDetailsPane;
    ReleaseCycleDetailsPane *m_releaseCycleDetailsPane;
    ReleaseDetailsPane *m_releaseDetailsPane;
    ProjectMainSubPage m_activePage;
};


#endif
