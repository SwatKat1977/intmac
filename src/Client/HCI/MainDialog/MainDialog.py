#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import wx
from AssetsPanel.AssetsPanel import *
from TestPlanPanel.TestPlanPanel import *
from ManageProjectPanel.ManageProjectPanel import *


class MainDialogPage(object): # enum CurrentPageSelected
	Assets = 0
	TestPlan = 1
	ManageProject = 2


# ***********************************************************************
# Implementation of the main dialog window class.
# ***********************************************************************
class MainDialog(wx.Frame):

	# Initial creation default values
	dialogWidth  = 800
	dialogHeight = 600
	dialogStyle = wx.CLOSE_BOX | wx.CAPTION | wx.SYSTEM_MENU | wx.RESIZE_BORDER
	defaultDialogName = "Integrated Test Manager Client" 

	ManageProjectImg = "Media/MainToolbar/ManageProject.png"
	ProjectAssetsImg = "Media/MainToolbar/projectAssets.png"
	TestPlanImg = "Media/MainToolbar/TestPlan.png" 

	# Toolbar Button Control ID's
	ID_toolbarProjectAssets = 0x110
	ID_toolbarTestPlan = 0x120
	ID_toolbarManageProject = 0x130

	# Event ID's for main dialog.
	EventID_updateTimer = 0x100

	# Update timer interval 
	UpdateTimerInterval = 2000


	def __init__(self, projectModel, stateModel, eventHandler):
		super(MainDialog, self).__init__(None, title = self.defaultDialogName
		,  size = (self.dialogWidth, self.dialogHeight)
		, style = self.dialogStyle)

 		self._projectModel = projectModel
 		self._stateModel = stateModel
 		self._eventHandler = eventHandler
 	
 		# Get the client size
		clientSize = self.GetClientSize()

		# Create Sizer for layout
		self._sizer = wx.BoxSizer(wx.VERTICAL)
		self.SetSizer(self._sizer)
		
		# Assets Panel
		self._assetsPanel = AssetsPanel(self, clientSize, self._projectModel)
		self._sizer.Add(self._assetsPanel, 1, wx.GROW)

		# Create Test Plan Panel
		self._testPlanPanel = TestPlanPanel(self)
		self._testPlanPanel.Hide()

		# Create Manage Project Panel
		self._manageProjectPanel = ManageProjectPanel(self, clientSize
			, projectModel, eventHandler)
		self._manageProjectPanel.Hide()

		# Set the active page as 'assets panel'.
		self._activePage = MainDialogPage.Assets
		
		#  -------------------
		#  Status Bar creation
		#  -------------------
		self._statusBar = self.CreateStatusBar()
		self._statusBar.SetFieldsCount(2)
 
		userStatus = "User: {0}\\{1}".format('testRealm', 'user') 
		# stateModel->GetRealm(), stateModel->GetUser()
		self._statusBar.SetStatusText(userStatus, 0)

		projectStatus = "Project: {0}".format('testProjet') 
		# stateModel->GetSelectedProjectName()
		self._statusBar.SetStatusText(projectStatus, 1)

		#  ----------------
		#  Toolbar creation
		#  ----------------
		self._toolbar = self.CreateToolBar(wx.TB_HORIZONTAL)
		self._toolbar.SetToolBitmapSize(wx.Size( 24, 24 ))

		# Load the 'Manage Project' icon and add entry to toolbar.
		manageProjectIcon = wx.Bitmap(self.ManageProjectImg, wx.BITMAP_TYPE_PNG)
		t_manageProject = self._toolbar.AddTool(self.ID_toolbarManageProject
			, manageProjectIcon, wx.NullBitmap, wx.ITEM_NORMAL
			, "Manage the project", "Manage the project")

		# Load the 'Project Assets' icon and add entry to toolbar.
		projectAssetsIcon = wx.Bitmap(self.ProjectAssetsImg, wx.BITMAP_TYPE_PNG)
		t_projectAssets = self._toolbar.AddTool(self.ID_toolbarProjectAssets
			, projectAssetsIcon, wx.NullBitmap, wx.ITEM_NORMAL
			, "Project Assets", "Project Assets")

		# Load the 'Test Plan' icon and add entry to toolbar.
		testPlanIcon= wx.Bitmap(self.TestPlanImg, wx.BITMAP_TYPE_PNG)
		t_testPlan = self._toolbar.AddTool(self.ID_toolbarTestPlan
			, testPlanIcon, wx.NullBitmap, wx.ITEM_NORMAL, "Test Plan"
			, "Test Plan")

		# Realise (draw) the toolbar.
		self._toolbar.Realize()

		# This is in the original code.. required????
		#Sleep(400)
		self.OnTimerTick(None)

		# Create update timer
		self._updateTimer = wx.Timer(self, id = self.EventID_updateTimer)
		self._updateTimer.Start(self.UpdateTimerInterval, wx.TIMER_CONTINUOUS)

		# ----------------
		#  Bind events to functions
		#  ----------------
		self.Bind(wx.EVT_TIMER, self.OnTimerTick)
		self.Bind(wx.EVT_CLOSE, self.OnCloseApplication)

		# Bind toolbar events
		self.Bind(wx.EVT_TOOL, self.OnToolbarManageProject, t_manageProject)
		self.Bind(wx.EVT_TOOL, self.OnToolbarProjectAssets, t_projectAssets)
		self.Bind(wx.EVT_TOOL, self.OnToolbarTestPlan, t_testPlan)


	##
	# Event when the 'Project Assets' toolbar item is selected.  The previous
	# panel is detached and hidden and the Project Assets panel is attached to
	# the sizer and then set active.
	# @event Required, but not used.	
	def OnToolbarProjectAssets(self, event):		
		#  Project Assets is already selected.. ignore it!
		if self._activePage == MainDialogPage.Assets:
			return

		# If the current page is 'Test Plan' then hide it.	
		if self._activePage == MainDialogPage.TestPlan:
			self._sizer.Detach(0)
			self._testPlanPanel.Hide()
			self._sizer.Prepend(self._assetsPanel, 1, wx.GROW)
			self._assetsPanel.Show()

		# If the current page is 'Manage Project' then hide it.	
		elif self._activePage == MainDialogPage.ManageProject:
			self._sizer.Detach(0)
			self._manageProjectPanel.Hide()
			self._sizer.Prepend(self._assetsPanel, 1, wx.GROW)
			self._assetsPanel.Show()

		self._activePage = MainDialogPage.Assets
		self._sizer.Layout()
		self._assetsPanel.Refresh()


	##
	# Event when the 'Test Plan' toolbar item is selected.  The previous panel
	# is detached and hidden and the Test Plan panel is attached to the sizer
	# and then set active.
	# @event Required, but not used.	
	def OnToolbarTestPlan(self, event):
		#  If current active page is 'Test Plan'.. ignore it!
		if self._activePage == MainDialogPage.TestPlan: return

		# Detach current page.
		self._sizer.Detach(0)
		
		# If the current page is 'Assets' then hide it.	
		if self._activePage == MainDialogPage.Assets:
			self._assetsPanel.Hide()

		# If the current page is 'Manage Project' then hide it.	
		elif self._activePage == MainDialogPage.ManageProject:
			self._manageProjectPanel.Hide()

		# Add the Test Plan panel to the sizer control.
		self._sizer.Prepend(self._testPlanPanel, 1, wx.GROW)

		# Set the Test Plan to be displayed.
		self._testPlanPanel.Show()

		# Set the current activate page
		self._activePage = MainDialogPage.TestPlan
		
		# Update the sizer control and refresh.
		self._sizer.Layout()
		self._manageProjectPanel.Refresh()


	##
	# Event when the 'Manage Project' toolbar item is selected.  The previous
	# panel is detached and hidden and the Manage Project panel is attached to
	# the sizer and then set active.
	# @event Required, but not used.	
	def OnToolbarManageProject(self, event):
		#  If current active page is 'Manage Project'.. ignore it!
		if self._activePage == MainDialogPage.ManageProject: return False

		# If the current page is 'Assets' then hide it.
		if self._activePage == MainDialogPage.Assets:
			self._sizer.Detach(0)
			self._assetsPanel.Hide()
			self._sizer.Prepend(self._manageProjectPanel, 1, wx.GROW)
			self._manageProjectPanel.Show()

		# If the current page is 'Test Plan' then hide it.	
		elif self._activePage == MainDialogPage.TestPlan:
			self._sizer.Detach(0)
			self._testPlanPanel.Hide()
			self._sizer.Prepend(self._manageProjectPanel, 1, wx.GROW)
			self._manageProjectPanel.Show()

		self._activePage = MainDialogPage.ManageProject
		self._sizer.Layout()
		self._manageProjectPanel.Refresh()


	##
	# Event when the timer ticks, the panels are updated.
	# @event Required, but not used.		
	def OnTimerTick(self, event):
		#self._assetsPanel.UpdatePage()
		#self._manageProjectPanel.UpdatePage()
		pass


	##
	# Event when the main dialog is closed.  Ensure that the update timer is
	# stopped.
	# @event Required, but not used.		
	def OnCloseApplication(self, event):
		self._updateTimer.Stop()
		self.Destroy()
