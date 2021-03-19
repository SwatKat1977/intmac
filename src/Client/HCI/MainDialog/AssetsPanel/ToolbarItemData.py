#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import wx


#include <wx/treectrl.h>


class ToolbarItemSubType(object):
	Unknown = 0
	TestingAssetsFilter = 1
	TestingAssetsTestItem = 2
	TestingAssetsTestGroup = 3


class ToolbarItemData(wx.TreeItemData):

	def __init__(self,id, subType, name):
		self._itemID = id
		self._subType = subType
		self._name = name


	def GetID(self): return self._itemID

	def GetNodeSubType(self): return self._subType

	def GetName(self): return self._name
