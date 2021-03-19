#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import wx
import wx.aui
from ToolbarPane import *
from MainPane import *


# ***********************************************************************
# Implementation of the assets panel class
# ***********************************************************************
class AssetsPanel(wx.Panel):

	toolbarDefaultWidth = 200
	
	
	def __init__(self, parent, clientSize, projectModel):
		super(AssetsPanel, self).__init__(parent, id=wx.ID_ANY, size=clientSize)
		self._projectModel = projectModel

		clientSize = self.GetClientSize()

		# Build toolbar size - default width and height.
		toolbarSize = wx.Size(self.toolbarDefaultWidth, clientSize.GetHeight())
		
		# Create toolbar pane (left-hand side).
		toolbarPane = ToolbarPane(self, toolbarSize)
    
		# Create main pane (right-hand side).          
		mainPane = MainPane(self)

		# Create Aui Manager and add the two panels.
		self._paneManager = wx.aui.AuiManager(self)
		self._paneManager.AddPane(toolbarPane, wx.LEFT)
		self._paneManager.AddPane(mainPane, wx.CENTER)

		#  ============================
		#  Update Toolbar pane settings
		#  ============================
		retrievedToolbarPane = self._paneManager.GetPane(toolbarPane)
		retrievedToolbarPane.Floatable(False)
		retrievedToolbarPane.CaptionVisible(False)
		retrievedToolbarPane.BestSize(toolbarPane.GetClientSize())

		self._paneManager.Update()

"""



void AssetsPage::UpdatePage()
{
    AssetDataTransaction *transaction = NULL;
    int transactionsWaiting = m_projectModel->GetNumberOfAssetTransactions();

    if ( transactionsWaiting > 0 )
    {
        for ( int transNo = 0; transNo < transactionsWaiting; transNo++ )
        {
            transaction = m_projectModel->GetTopAssetTransaction();

            switch( transaction->GetType() )
            {
            case AssetDataTransactionType_add:
                m_toolbarPane->AddAssetItem( transaction );
                m_projectModel->PopTopAssetTransaction();
                break;

            default:
                break;
            }
        }
    }
}


void AssetsPage::Refresh()
{
    m_toolbarPane->Refresh();
}

"""
