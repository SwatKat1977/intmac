#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import wx


class ReleaseCyclePanel(wx.Panel): 


	def __init__(self, parent, panelSize):

		super(ReleaseCyclePanel, self).__init__(parent,  size = panelSize)

		topSizer = wx.BoxSizer(wx.VERTICAL)
		self.SetSizer(topSizer)
	
		##################################
		# Release name label and and input box
		##################################
		controlGrid = wx.FlexGridSizer(0, 2, 0, 0)
		controlGrid.SetFlexibleDirection(wx.BOTH)
		controlGrid.SetNonFlexibleGrowMode(wx.FLEX_GROWMODE_SPECIFIED)
	
		topSizer.Add(controlGrid, 1, wx.ALL | wx.EXPAND, 5)
	
		labelName = wx.StaticText(self, wx.ID_ANY, "Name:", wx.DefaultPosition
			, wx.Size(-1, -1), wx.ALIGN_RIGHT)
		controlGrid.Add(labelName, 0, wx.ALL | wx.ALIGN_RIGHT, 5)
	
		txtName = wx.TextCtrl(self, wx.ID_ANY, "", wx.DefaultPosition
			, wx.Size(200, -1), 0)
		#txtName.SetToolTip("Required release name")
		controlGrid.Add(txtName, 0, wx.ALL, 5)
	
		##################################
		# Release ID label and and input box
		##################################
		labelReleaseID = wx.StaticText(self, wx.ID_ANY, "Release ID:"
			, wx.DefaultPosition, wx.Size(-1, -1), wx.ALIGN_RIGHT)
		controlGrid.Add(labelReleaseID, 0, wx.ALL | wx.ALIGN_RIGHT, 5)
	
		txtReleaseID = wx.TextCtrl(self, wx.ID_ANY, "", wx.DefaultPosition
			, wx.Size(200, -1), 0)
		#txtReleaseID.SetToolTip("Required release ID")
		controlGrid.Add(txtReleaseID, 0, wx.ALL, 5)
	
		##################################
		# Description label and and input box
		##################################
		labelDescription = wx.StaticText(self, wx.ID_ANY, "Description:"
			, wx.DefaultPosition, wx.Size(-1, -1), 0)
		controlGrid.Add(labelDescription, 0, wx.ALL, 5)
	
		txtDescription = wx.TextCtrl(self, wx.ID_ANY, "", wx.DefaultPosition
			, wx.Size(200, 200), wx.TE_NO_VSCROLL | wx.TE_MULTILINE)
		#txtDescription.SetToolTip("Optional description of folder")
		controlGrid.Add(txtDescription, 0, wx.ALL, 5)
