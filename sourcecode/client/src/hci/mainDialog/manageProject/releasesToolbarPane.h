//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef RELEASESTOOLBARPANE_H
#define RELEASESTOOLBARPANE_H

#include <wx/wx.h>
#include <hci/mainDialog/manageProject/releasesTreeCtrl.h>
#include <dataModels/releaseTransaction.h>


class ReleasesToolbarPane : public wxPanel
{
public:
    ReleasesToolbarPane( wxWindow *parent
                       , const wxSize &size
                       , iEventHandler *eventHandler );

    ~ReleasesToolbarPane();

    void AddItem( ReleaseTransaction * transaction );

private:
    ReleasesTreeCtrl *m_treeCtrl;
};

#endif
