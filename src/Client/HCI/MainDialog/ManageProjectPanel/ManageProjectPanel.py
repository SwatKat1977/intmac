#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import wx
from ReleasesToolbarPanel import *
from ManageProjectMain import *


class ManageProjectPanel(wx.Panel): 

	PanelStyle = wx.TAB_TRAVERSAL
	DefaultToolbarWidth = 150


	def __init__(self, parent, dialogSize, projectModel, eventHandler):
		super(ManageProjectPanel, self).__init__(parent
			,  size = dialogSize, style = self.PanelStyle)

		self._projectModel = projectModel
		self._eventHandler = eventHandler

		# Get the size of the client area
		clientSize = self.GetClientSize()

		# Create the toolbar using the default size.
		toolbarSize = wx.Size(self.DefaultToolbarWidth, clientSize.GetHeight())
		self._toolbarPanel = ReleasesToolbarPanel(self, toolbarSize
			, eventHandler)

		self._manageProjectMain = ManageProjectMain(self, eventHandler)

		# Create Aui Manager and add the two panels.
		self._paneManager = wx.aui.AuiManager(self)
		self._paneManager.AddPane(self._toolbarPanel, wx.LEFT)
		self._paneManager.AddPane(self._manageProjectMain, wx.CENTER)

		#  ============================
		#  Update Toolbar pane settings
		#  ============================
		retrievedToolbarPane = self._paneManager.GetPane(self._toolbarPanel)
		retrievedToolbarPane.Floatable(False)
		retrievedToolbarPane.CaptionVisible(False)
		retrievedToolbarPane.BestSize(self._toolbarPanel.GetClientSize())
		self._paneManager.Update()


	def __exit__(self ,type, value, traceback):
		self._paneManager.UnInit()


	def Refresh(self):
		self._toolbarPanel.Refresh()


"""
void ManageProjectPage::UpdatePage()
{
    ReleaseTransaction *transaction = NULL;
    int transactionsWaiting = m_projectModel->GetNumberOfReleaseTransactions();

    while( m_projectModel->GetNumberOfReleaseTransactions() != 0 )
    {
        transaction = m_projectModel->GetTopReleaseTransaction();

        switch( transaction->GetTransactionType() )
        {
        case ReleaseTransactionType_add:
            m_toolbarPane->AddItem( transaction );
            m_projectModel->PopTopReleaseTransaction();
            break;

        default:
            break;
        }
    }
}

"""
