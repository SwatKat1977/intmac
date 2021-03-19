#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import wx
from ReleasesTreeCtrl import *


#include <hci/mainDialog/ManageProject/releasesToolbarPane.h>
#include <hci/mainDialog/manageProject/releasesTreeCtrl.h>
#include <dataModels/releaseTransaction.h>


class ReleasesToolbarPanel(wx.Panel):

	ID_ReleasesToolbarPanel = 0x100


	def __init__(self, parent, panelSize, eventHandler):
		super(ReleasesToolbarPanel, self).__init__(parent
			, self.ID_ReleasesToolbarPanel,  size = panelSize)

		topSizer = wx.BoxSizer(wx.HORIZONTAL)

		self._treeCtrl = ReleasesTreeCtrl( self
			, panelSize, eventHandler)

		topSizer.Add(self._treeCtrl, wx.EXPAND, wx.EXPAND)

		
		self.SetSizerAndFit(topSizer)
	
	"""
	void ReleasesToolbarPane::AddItem(ReleaseTransaction *transaction):
	{
		m_treeCtrl->AddTreeItem(transaction->GetID()
			, transaction->GetParentID()
			, transaction->GetTitle()
			, transaction->GetItemType())
	"""