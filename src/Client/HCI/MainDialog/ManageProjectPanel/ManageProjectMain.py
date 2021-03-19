#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import wx
from ReleaseFolderPanel import *
from ReleaseCyclePanel import *
from ReleaseDetailsPanel import *


class ManageProjectPanel(object):
	FolderDetails = 0
	CycleDetails = 1
	ReleaseDetails = 2


class ManageProjectMain(wx.Panel): 

	def __init__(self, parent, eventHandler):
		super(ManageProjectMain, self).__init__(parent, wx.ID_ANY)

		# Get the window size, panels require it as a parameter.
		clientSize = self.GetClientSize()

		# Default active panel is 'Folder Details".
		self._activePage = ManageProjectPanel.FolderDetails

		self._sizer = wx.BoxSizer(wx.VERTICAL)
		self.SetSizer(self._sizer)

		# Create Release Folder Panel and add it to sizer control.
		self._releaseFolderPanel = ReleaseFolderPanel(self, clientSize)
		self._sizer.Add(self._releaseFolderPanel, 1, wx.GROW)

		# Create Release Cycle Panel and hide as not active panel.
		self._releaseCyclePanel = ReleaseCyclePanel(self, clientSize)
		self._releaseCyclePanel.Hide()

		# Create Release Details Panel and hide as not active panel.
		self._releaseDetailsPanel = ReleaseDetailsPanel(self, clientSize)
		self._releaseDetailsPanel.Hide();

		# eventHandler.RegisterObserver(self)


"""
void ManageProjectMain::SelectSubPageFolderDetails()
{
    switch( m_activePage )
    {
    //  This is already selected.. ignore it!
    case ProjectMainSubPage_folderDetails:
        break;

    case ProjectMainSubPage_cycleDetails:
        m_sizer->Detach( 0 ); 
        m_releaseCycleDetailsPane->Hide();
        m_sizer->Prepend( m_releaseFolderDetailsPane, 1, wxGROW );
        m_releaseFolderDetailsPane->Show();
        break;

    case ProjectMainSubPage_releaseDetails:
        m_sizer->Detach( 0 ); 
        m_releaseDetailsPane->Hide();
        m_sizer->Prepend( m_releaseFolderDetailsPane, 1, wxGROW );
        m_releaseFolderDetailsPane->Show();
        break;
    }

    m_activePage = ProjectMainSubPage_folderDetails;
    m_sizer->Layout();
    m_releaseFolderDetailsPane->Refresh();
}


void ManageProjectMain::SelectSubPageCycleDetails()
{
    switch( m_activePage )
    {
    //  This is already selected.. ignore it!
    case ProjectMainSubPage_cycleDetails:
        break;

    case ProjectMainSubPage_folderDetails:
        m_sizer->Detach( 0 ); 
        m_releaseFolderDetailsPane->Hide();
        m_sizer->Prepend( m_releaseCycleDetailsPane, 1, wxGROW );
        m_releaseCycleDetailsPane->Show();
        break;

    case ProjectMainSubPage_releaseDetails:
        m_sizer->Detach( 0 ); 
        m_releaseDetailsPane->Hide();
        m_sizer->Prepend( m_releaseCycleDetailsPane, 1, wxGROW );
        m_releaseCycleDetailsPane->Show();
        break;
    }

    m_activePage = ProjectMainSubPage_cycleDetails;
    m_sizer->Layout();
    m_releaseCycleDetailsPane->Refresh();
}


void ManageProjectMain::SelectSubPageReleaseDetails()
{
    switch( m_activePage )
    {
    //  This is already selected.. ignore it!
    case ProjectMainSubPage_releaseDetails:
        break;

    case ProjectMainSubPage_cycleDetails:
        m_sizer->Detach( 0 ); 
        m_releaseCycleDetailsPane->Hide();
        m_sizer->Prepend( m_releaseDetailsPane, 1, wxGROW );
        m_releaseDetailsPane->Show();
        break;

    case ProjectMainSubPage_folderDetails:
        m_sizer->Detach( 0 ); 
        m_releaseFolderDetailsPane->Hide();
        m_sizer->Prepend( m_releaseDetailsPane, 1, wxGROW );
        m_releaseDetailsPane->Show();
        break;
    }

    m_activePage = ProjectMainSubPage_releaseDetails;
    m_sizer->Layout();
    m_releaseDetailsPane->Refresh();
}


void ManageProjectMain::ReceiveEventMessage( EventID ID
                                           , iEventMsg *msg )
{
    switch( ID )
    {
    case Evt_Releases_Release:
        SelectSubPageReleaseDetails();
        break;

    case Evt_Releases_ReleaseFolder:
        SelectSubPageFolderDetails();
        break;

    case Evt_Releases_ReleaseCycle:
        SelectSubPageCycleDetails();
        break;

    default:
        break;
    }
}
"""
