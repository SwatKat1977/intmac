#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////


class ReleaseItemData:


	def __init__(self, id, itemName, releaseItemType):
		self._itemID = id
		self._itemName = itemName
		self._releaseItemType = releaseItemType


	def GetID(self):
		return self._itemID


	def GetName(self):
		return self._itemName


	def GetType(self):
		return self._releaseItemType
