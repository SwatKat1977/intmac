#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import wx
from AssetsTreeCtrl import *
#include <dataModels/assetDataTransaction.h>


#const wxWindowID ID_toolbarPane = 0x010;

# wxID_ANY

class ToolbarPane(wx.Panel):

	def __init__(self, parent, size):
		super(ToolbarPane, self).__init__(parent, wx.ID_ANY, 
				wx.DefaultPosition, size)

		self._topLevelSizer = wx.BoxSizer(wx.HORIZONTAL)

		self._treeCtrl = AssetsTreeCtrl(self, size)

		self._topLevelSizer.Add(self._treeCtrl, wx.EXPAND, wx.EXPAND)

		self.SetSizerAndFit(self._topLevelSizer)

"""
	void ToolbarPane::AddAssetItem( AssetDataTransaction *transaction )

		m_treeCtrl->AddAssetItem( transaction->GetID()
                            , transaction->GetParentID()
                            , transaction->GetTitle()
                            , transaction->GetAssetData()->GetType() );
"""
