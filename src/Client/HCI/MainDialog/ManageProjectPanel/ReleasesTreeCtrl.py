#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import wx
from ReleaseItemData import *
from Common.CommonDefinitions import *


class ReleasesTreeCtrl(wx.TreeCtrl):

	TreeStyle = wx.TR_DEFAULT_STYLE | wx.TR_EDIT_LABELS
	
	# Right-click context menu
	CtxMenuNewReleaseFolder = "New Release Folder"
	CtxMenuItemNewRelease = "New Release"
	CtxMenuItemNewReleaseCycle = "New Cycle"

	# Root constants
	RootName = "Releases"
	RootItemID = 0x0
	IconIndex_root = -1

	# Right-click context menu control ID's 
	MenuIdNewReleaseFolder = wx.ID_HIGHEST + 1
	MenuIdNewRelease = wx.ID_HIGHEST + 2
	MenuIdNewReleaseCycle = wx.ID_HIGHEST + 3


	def __init__(self, parent, dialogSize, eventHandler):
		super(ReleasesTreeCtrl, self).__init__(parent, size = dialogSize
			, style = self.TreeStyle)

		# Define the dictionary to map the tree items data to it's ID.
		self._wxtreeToReleaseMap = {}
		
		self._eventHandler = eventHandler

		# Initiate current selected to 'None'.
		self._selectedItem = None

		# Create the root object for the releases tree and the extra data
		# associated with it before adding it.
		rootData = ReleaseItemData(self.RootItemID, self.RootName
			, ReleaseItemType.Folder)
		self._rootItem = self.AddRoot(self.RootName, self.IconIndex_root
			, self.IconIndex_root)
		self.SetPyData(self._rootItem, rootData)

		self._wxtreeToReleaseMap[self.RootItemID] = self._rootItem
	
		# Bind event when a user right-click a tree item.  A context menu is
		# created depending on the item type.
		self.Bind(wx.EVT_TREE_ITEM_RIGHT_CLICK, self.OnRightClick)

		# Bind event when an item in the tree is selected.
		self.Bind(wx.EVT_TREE_SEL_CHANGED, self.OnSelectItem)

		# Bind event when a label for a tree item edit event is activated.
		self.Bind(wx.EVT_TREE_BEGIN_LABEL_EDIT, self.OnItemLabelEdit)

		# Bind event when a right-click context menu is selected.
		self.Bind(wx.EVT_MENU, self.OnContextMenu)

	
	##
	# Event when a right-click context menu item is selected.
	# @self Class instance.
	# @event Context menu item.
	def OnContextMenu(self, event):
		# Get the ID of the event (control ID)
		eventID = event.GetId()
		
		# 'New Release Folder' menu item.
		if eventID == self.MenuIdNewReleaseFolder:
			#self.OnAddTestingFilter(event)
			pass

		elif eventID == self.MenuIdNewRelease:
			pass

		elif eventID == self.MenuIdNewReleaseCycle:
			pass


	def AddTreeItem(self, itemID, parentItemID, label, releaseItemType):
		parentTreeItem = self._wxtreeToAssetMap[parentItemID]
		treeIconIndex = -1

		data = ReleaseItemData(itemID, label, releaseItemType)

		itemTreeID = AppendItem(parentTreeItem, label, treeIconIndex
			, treeIconIndex, data)

		self._wxtreeToReleaseMap[itemID] = itemTreeID;


	def OnRightClick(self, event):
		itemData = None
		menu = wx.Menu()

		#  Ensure that right-clicked item is in focus by forcably selecting it.
		self.SelectItem(event.GetItem())

		self._selectedItem = self.GetSelection()

		if self._selectedItem.IsOk():
			# Get the additional data associated with selected item.
			itemData = self.GetPyData(self._selectedItem)

			if itemData.GetType() == ReleaseItemType.Folder:
				menu.Append(self.MenuIdNewReleaseFolder
					, self.CtxMenuNewReleaseFolder)
				menu.Append(self.MenuIdNewRelease
					, self.CtxMenuItemNewRelease)

			elif itemData.GetType() == ReleaseItemType.Release:
				menu.Append(self.MenuIdNewReleaseCycle, self.CtxMenuItemNewReleaseCycle)

			elif itemData.GetType() == ReleaseItemType.Cycle:
				pass	# Not implemented yet.

			# Display the popup (context) menu.
			self.PopupMenu(menu)


	def OnSelectItem(self, event):
		itemData = None
		menu = wx.Menu()

		print("here")

		#  Ensure that right-clicked item is in focus by forcably selecting it.
		SelectItem(event.GetItem())

		self._selectedItem = self.GetSelection()

		if self._selectedItem.IsOk():
			# Get the additional data associated with selected item.
			itemData = self.GetPyData(self._selectedItem)

			if itemData.GetType() == ReleaseItemType_folder:
				pass
				# m_eventHandler->ReceiveMessage(Evt_Releases_ReleaseFolder)

			elif itemData.GetType() == ReleaseItemType_release:
				pass
				# m_eventHandler->ReceiveMessage(Evt_Releases_Release)

			elif itemData.GetType() == ReleaseItemType_cycle:
				pass
				# m_eventHandler->ReceiveMessage(Evt_Releases_ReleaseCycle)


	def OnItemLabelEdit(self, event):

		# Ensure that right-clicked item is in focus by forcably selecting it.
		self.SelectItem(event.GetItem())

		self._selectedItem = self.GetSelection()

		if self._selectedItem.IsOk() == False:
			event.Veto()
			return

		itemData = self.GetPyData(self._selectedItem)
	
		# Veto label editing if selected item is the tree root.
		if itemData.GetID() == self.RootItemID: event.Veto()
