#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import wx
from ToolbarItemData import *
from Client.HCI.CommonInputBox import *


# Context menu ID's
MENUID_addFilter = wx.ID_HIGHEST + 1
MENUID_deleteFilter = wx.ID_HIGHEST + 2
MENUID_addTestItem = wx.ID_HIGHEST + 3
MENUID_deleteTestItem = wx.ID_HIGHEST + 4
MENUID_addTestGroup = wx.ID_HIGHEST + 5
MENUID_deleteTestGroup = wx.ID_HIGHEST + 6

# Context menu names
ContextMenuItem_addFilter = "Add Filter"
ContextMenuItem_deleteFilter = "Delete Filter"
ContextMenuItem_addTestItem = "Add Test Item"
ContextMenuItem_deleteTestItem = "Delete Test Item"
ContextMenuItem_addTestGroup = "Add Test Group"
ContextMenuItem_deleteTestGroup = "Delete Test Group"


# This order should coincide with order they are added to the image list.
class ToolbarIconIndex(object):
	Filter = 0
	TestItem = 1
	TestGroup = 2


# ***********************************************************************
# Implementation of the assets tree control class
# ***********************************************************************
class AssetsTreeCtrl(wx.TreeCtrl):

	# Tree icon names and paths
	FilterIconImg = "Media/Icons/FilterIcon.png"
	TestItemIconImg = "Media/Icons/TestItemIcon.png"
	TestGroupIconImg = "Media/Icons/TestGroupIcon.png"

	TreeRootName = "Project Assets"
	TreeRootItemID = 0x0


	def __init__(self, parent, clientSize):
		super(AssetsTreeCtrl, self).__init__(parent, wx.ID_ANY, size=clientSize
			, style=wx.TR_DEFAULT_STYLE | wx.TR_EDIT_LABELS)

		self._selectedItem = None
		
		self._wxtreeToAssetMap = {}
		
		# Temporary...
		self._nextID = 4000
		
		# Image list that holds the tree icon images in.
		self._treeIcons = wx.ImageList(32, 32)

		# Load 'Filter' icon
		filterIcon = wx.Bitmap(self.FilterIconImg, wx.BITMAP_TYPE_PNG)
		
		# Load 'Test Item' icon
		testItemIcon = wx.Bitmap(self.TestItemIconImg, wx.BITMAP_TYPE_ANY)

		# Load 'Test Group' icon
		testGroupIcon= wx.Bitmap(self.TestGroupIconImg, wx.BITMAP_TYPE_ANY)

		# Add the images to the image list.
		self._treeIcons.Add(filterIcon)
		self._treeIcons.Add(testItemIcon)
		self._treeIcons.Add(testGroupIcon)

		# Create the root object and associated data class.
		rootData = ToolbarItemData(self.TreeRootItemID
								, ToolbarItemSubType.Unknown
								, self.TreeRootName)
		self._treeRoot = self.AddRoot(self.TreeRootName
								, ToolbarIconIndex.Filter
								, ToolbarIconIndex.Filter)
		self.SetPyData(self._treeRoot, rootData)
						
		self.AssignImageList(self._treeIcons)

		self._wxtreeToAssetMap[self.TreeRootItemID] = self._treeRoot
	
		self.Bind(wx.EVT_TREE_ITEM_RIGHT_CLICK, self.OnTreeRightClick)
	
		self.Bind(wx.EVT_TREE_BEGIN_LABEL_EDIT, self.OnItemLabelEdit)

		self.Bind(wx.EVT_MENU, self.OnContextMenu)


	##
	# Event when a right-click context menu item is selected.
	# @self Class instance.
	# @event Context menu item.
	def OnContextMenu(self, event):
		# Get the ID of the event (control ID)
		eventID = event.GetId()
		
		# 'Add filter' menu item.
		if eventID == MENUID_addFilter:
			self.OnAddFilter(event)

		elif eventID == MENUID_deleteFilter:
			self.OnDeleteFilter(event)

		elif eventID == MENUID_addTestItem:
			self.OnAddTestItem(event)

		elif eventID == MENUID_deleteTestItem:
			pass
			
		elif eventID == MENUID_addTestGroup:
			self.OnAddTestGroup(event)

		elif eventID == MENUID_deleteTestGroup:
			pass


	# This should replace AddTreeItem() eventually
	def AddAssetItem(self, itemID, parentItemID, label, itemType):
		parentTreeItem = self._wxtreeToAssetMap[parentItemID]
		treeIconIndex = -1
		subType = None

		if itemType == ToolbarItemSubType.TestingAssetsFilter:
			treeIconIndex = ToolbarIconIndex.Filter
			subType = ToolbarItemSubType.TestingAssetsFilter

		elif itemType == ToolbarItemSubType.TestingAssetsTestItem:
			treeIconIndex = ToolbarIconIndex.TestItem
			subType = ToolbarItemSubType.TestingAssetsTestItem

		elif itemType == ToolbarItemSubType.TestingAssetsTestGroup:
			treeIconIndex = ToolbarIconIndex.TestGroup
			subType = ToolbarItemSubType.TestingAssetsTestGroup

		else:
			subType = ToolbarItemSubType.TestingAssetsFilter
			treeIconIndex = ToolbarIconIndex.Filter

		# Add new tree item and associate item data with it.
		newTreeItem = self.AppendItem(parentTreeItem, label, treeIconIndex
			, treeIconIndex)
		data = ToolbarItemData(itemID, subType, label)
		self.SetPyData(newTreeItem, data)

		self._wxtreeToAssetMap[itemID] = newTreeItem


	def OnAddTestItem(self, event):
		selectedItem = self.GetSelection()
		
		if selectedItem.IsOk() == False: return

		# Get the additional data associated with selected item.
		itemData = self.GetPyData(selectedItem)

		#CommonInputBox *filterInput = NULL;
		settings = CommonInputBoxSettings()

		settings._tooltip = "Alpha numeric or numbers only permitted.\
                              \nMinimum of 1 character"
		settings._excludedCharacters = "!@#$%^&*={}~"
		settings._minimumLength = 1

		filterInput = CommonInputBox(self, "Enter Test Item name"
				, "Test Item Name:", settings)

		self._nextID += 1 
		
		if filterInput.ShowModal() == wx.ID_OK:
			self.AddAssetItem(self._nextID, itemData.GetID()
				, filterInput.GetInputValue()
				, ToolbarItemSubType.TestingAssetsTestItem)


	def OnAddTestGroup(self, event):
		selectedItem = self.GetSelection()
		
		if selectedItem.IsOk() == False: return

		# Get the additional data associated with selected item.
		itemData = self.GetPyData(selectedItem)

		settings = CommonInputBoxSettings()
		settings._tooltip = "Alpha numeric or numbers only permitted.\
                              \nMinimum of 1 character"
		settings._excludedCharacters = "!@#$%^&*={}~"
		settings._minimumLength = 1

		filterInput = CommonInputBox(self, "Enter Test Group name"
				, "Test Group Name:", settings)

		self._nextID += 1 
		
		if filterInput.ShowModal() == wx.ID_OK:
			self.AddAssetItem(self._nextID, itemData.GetID()
				, filterInput.GetInputValue()
				, ToolbarItemSubType.TestingAssetsTestGroup)


	def OnTreeRightClick(self, event):
		itemData = None
		menu = wx.Menu()

		#  Ensure that right-clicked item is in focus by forcably selecting it.
		self.SelectItem(event.GetItem())

		self._selectedItem = self.GetSelection()

		if self._selectedItem.IsOk():
			# Get the additional data associated with selected item.
			itemData = self.GetPyData(self._selectedItem)

			# Get the node subtype, this is influence what menu items are
			# available on the right-click menu.
			subType = itemData.GetNodeSubType()
			
			# The root object can't be deleted, but other operations such as
			# creating a filter, test item or test group is permitted.
			if itemData.GetID() == self.TreeRootItemID:
				menu.Append(MENUID_addFilter, ContextMenuItem_addFilter)
				menu.AppendSeparator()
				menu.Append(MENUID_addTestItem, ContextMenuItem_addTestItem)
				menu.AppendSeparator();
				menu.Append(MENUID_addTestGroup, ContextMenuItem_addTestGroup)
  
  			# If the subtype is a filter then it can be deleted, a child one,
  			# test item or group can be created.
			elif subType == ToolbarItemSubType.TestingAssetsFilter:
				menu.Append(MENUID_deleteFilter, ContextMenuItem_deleteFilter)
				menu.Append(MENUID_addFilter, ContextMenuItem_addFilter)
				menu.AppendSeparator()
				menu.Append(MENUID_addTestItem, ContextMenuItem_addTestItem)
				menu.AppendSeparator();
				menu.Append(MENUID_addTestGroup, ContextMenuItem_addTestGroup)

  			# If the subtype is a test item then the only permitted action is
  			# for it to be deleted.
			elif subType == ToolbarItemSubType.TestingAssetsTestItem:
				menu.Append(MENUID_deleteTestItem, ContextMenuItem_deleteTestItem)

  			# If the subtype is a test group then the group can be deleted or a
  			# test item created.
			elif subType == ToolbarItemSubType.TestingAssetsTestGroup:
				menu.Append(MENUID_deleteTestGroup, ContextMenuItem_deleteTestGroup)
				menu.Append(MENUID_addTestItem, ContextMenuItem_addTestItem)

			# Display the popup (context) menu.
        	self.PopupMenu(menu)


	def OnAddFilter(self, event):
		tooltip = "Alpha numeric or numbers only permitted.  \
		Minimum of 1 character"
		
		if self._selectedItem.IsOk() == False: return

		# Get the additional data associated with selected item.
		itemData = self.GetPyData(self._selectedItem)

		#CommonInputBox *filterInput = NULL;
		settings = CommonInputBoxSettings()

		settings._tooltip = "Alpha numeric or numbers only permitted.\
                              \nMinimum of 1 character"
		settings._excludedCharacters = "!@#$%^&*={}~"
		settings._minimumLength = 1

		filterInput = CommonInputBox(self, "Enter new filter name"
				, "Filter Name:", settings)

		self._nextID += 1 
		
		if filterInput.ShowModal() == wx.ID_OK:
			self.AddAssetItem(self._nextID, itemData.GetID()
				, filterInput.GetInputValue()
				, ToolbarItemSubType.TestingAssetsFilter)


	def OnDeleteFilter(self, event):
		# Get the current selected tree item and verify it's OK.
		selectedItem = self.GetSelection()
		if selectedItem.IsOk() == False: return

		# Get the additional data associated with selected item.
		itemData = self.GetPyData(selectedItem)

		confirm = wx.MessageBox('Are you sure to want to delete it?'
        	, 'Confirm', wx.YES_NO | wx.NO_DEFAULT | wx.ICON_QUESTION)

		# If 'No' was clicked then just abort delete.
		if confirm == wx.NO: return

		if itemData.GetID() in self._wxtreeToAssetMap:
			del self._wxtreeToAssetMap[itemData.GetID()]

		# Delete the tree item.
		self.Delete(selectedItem)


	def OnItemLabelEdit(self, event):

		# Ensure that right-clicked item is in focus by forcably selecting it.
		self.SelectItem(event.GetItem())

		self._selectedItem = self.GetSelection()

		if self._selectedItem.IsOk() == False: return

		itemData = self.GetPyData(self._selectedItem)
	
		# Veto label editing if selected item is the tree root.
		if itemData.GetID() == self.TreeRootItemID: event.Veto()
