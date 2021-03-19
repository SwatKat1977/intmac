#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import wx


class ReleaseDetailsPanel(wx.Panel):


	def __init__(self, parent, panelSize):

		super(ReleaseDetailsPanel, self).__init__(parent,  size = panelSize)

		wx.StaticText(self, 121, "ReleaseDetailsPane")
