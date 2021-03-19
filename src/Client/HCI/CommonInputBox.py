#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import wx


class CommonInputBoxSettings(object):

	def __init__(self):
		self._minimumLength = 0
		self._maximumLength = 0
		self._excludedCharacters = ""
		self._tooltip = ""


class CommonInputBox(wx.Dialog):

	InputTextCtrlID = 9901


	def __init__(self, parent, title, label, settings):
		super(CommonInputBox, self).__init__(parent, wx.ID_ANY, title
			, wx.DefaultPosition, wx.Size(500, 90))

		# Store settings, these will be needed when OK is pressed.
		self._settings = settings
		
		# The sizer that encapsulates the rest of the dialog.
		self._topSizer = wx.BoxSizer(wx.VERTICAL)
		
		# Input line sizer.
		self._inputLineSizer = wx.BoxSizer(wx.HORIZONTAL)
		
		# Sizer of buttons.
		self._buttonsSizer = wx.BoxSizer(wx.HORIZONTAL)

		panel = wx.Panel(self, wx.ID_ANY)

		self._inputLabelCtrl = wx.StaticText(panel, wx.ID_ANY, label)
		
		self._textBoxCtrl = wx.TextCtrl(panel, self.InputTextCtrlID, "",\
			wx.DefaultPosition)

		if settings._maximumLength:
			m_inputTextBox.SetMaxLength(settings._maximumLength)

		self._textBoxCtrl.SetToolTip(wx.ToolTip(settings._tooltip))

		okButton = wx.Button(panel, wx.ID_ANY, "OK")
		cancelButton = wx.Button(panel, wx.ID_ANY, "Cancel")

		self._inputLineSizer.Add(self._inputLabelCtrl, wx.ALL, 5)
		self._inputLineSizer.Add(self._textBoxCtrl, wx.ALL|wx.EXPAND, 5)

		self._buttonsSizer.Add(okButton, 0, wx.ALL, 5)
		self._buttonsSizer.Add(cancelButton, 0, wx.ALL, 5)
		
		self._topSizer.Add(self._inputLineSizer, 0,wx.ALL, border=5)
		self._topSizer.Add(self._buttonsSizer, 0, wx.ALL|wx.CENTER, 5)

		panel.SetSizer(self._topSizer)

		# Bind click event to Cancel button
		cancelButton.Bind(wx.EVT_BUTTON, self.OnCancel)
	
		# Bind click event to OK button
		okButton.Bind(wx.EVT_BUTTON, self.OnOk)
	
		self.Center()


	def GetInputValue(self): 
		return self._textBoxCtrl.GetValue()


	def OnCancel(self, Event): 
		self.EndModal(wx.ID_CANCEL)
		self.Destroy()


	def OnOk(self, event):    

		inputString = self.GetInputValue()

		if len(inputString) < self._settings._minimumLength:
			errmsg = self._settings._tooltip;

			errDlg = wx.MessageDialog(None, errmsg, "Validation Error", wx.OK)
			errDlg.ShowModal()

		else:
			self.EndModal(wx.ID_OK)
			self.Destroy()
