#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import wx
from Common.ObserverClass import ObserverClass
from Common.Event import Event
from Common.EventManager import EventManager
from Client.EventIDs import *	

LogonDialogWidth = 300
LogonDialogHeight = 145
LogonDialogTitle = 'INTMAC Logon'
LogonDialogStyle = wx.CLOSE_BOX | wx.CAPTION | wx.SYSTEM_MENU

MsgStr_CancelQuestion = 'Are you sure you want to quit?'
MsgStr_CancelTitle = 'Confirm Quit'

CancelMsgBoxState = wx.YES_NO | wx.ICON_QUESTION


class LogonDialog(wx.Dialog, ObserverClass):

	# Update timer interval 
	UpdateTimerInterval = 2000


	class ControlID(object):
		UsernameTextCtrl = 101
		PasswordTextCtrl = 102
		AuthenticateBtnCtrl = 103

	## LogonDialog class constructor.
	# @param self The object pointer.
	def __init__(self, parent, systemObject):
		super(LogonDialog, self).__init__(None, title = LogonDialogTitle
		, size = (LogonDialogWidth, LogonDialogHeight)
		, style = LogonDialogStyle)

		# System object instance.
		self._systemObject = systemObject

		# Panel for the dialog.
		panel = wx.Panel(self, wx.ID_ANY)

		# The sizer that encapsulates the dialog.
		self._sizer = wx.GridBagSizer(0, 0)

		# Login Name Controls for the dialog
		######################
		loginNameLabelCtrl = wx.StaticText(panel, label = 'Login Name:')

		self._sizer.Add(loginNameLabelCtrl, pos = (0, 0), flag = wx.ALL,
			border = 5)
		self._loginNameCtrl = wx.TextCtrl(panel, 
			self.ControlID.UsernameTextCtrl, "")
		# m_TextUsername->SetMaxLength( 32 );

		self._sizer.Add(self._loginNameCtrl, pos = (0, 1), span = (1, 3), 
			flag = wx.EXPAND|wx.ALL, border = 5)

		# Password Controls for the dialog
		######################
		passwordLabelCtrl = wx.StaticText(panel, label = 'Password:')
		self._sizer.Add(passwordLabelCtrl, pos = (1, 0), flag = wx.ALL,
			border = 5)		
		self._passwordCtrl = wx.TextCtrl(panel, 
			self.ControlID.PasswordTextCtrl, "", style = wx.TE_PASSWORD)
		# m_TextPassword->SetMaxLength( 32 );
		self._sizer.Add(self._passwordCtrl, pos = (1, 1), span = (1, 3), 
			flag = wx.EXPAND|wx.ALL, border = 5)

		# Status Control for the dialog
		######################
		self._statusCtrl = wx.StaticText(panel, label = 'Unknown status...')
		self._sizer.Add(self._statusCtrl, pos = (2, 0), span = (1, 3),
			flag = wx.ALL, border = 5)		

		# Button Controls for the dialog
		######################
		# Logon button
		self._logonBtn = wx.Button(panel, self.ControlID.AuthenticateBtnCtrl,
			"Authenticate", wx.Point( 10, 85 ), wx.Size( 100, 25 ))
		self._sizer.Add(self._logonBtn, pos = (3, 2),  flag = wx.ALL,
			border = 5)
		self._logonBtn.Disable()

		# Make the 2nd column growable and set to fit.
		self._sizer.AddGrowableCol(1) 
		panel.SetSizerAndFit(self._sizer)

		self.Center()

		# ----------------
		#  Bind events to functions
		#  ----------------
		self.Bind(wx.EVT_CLOSE, self.OnClose)
		self.Bind(wx.EVT_BUTTON, self.OnAuthenticate)

		self._systemObject.RegisterObserver('ConnectedToGateway', self)
		self._systemObject.RegisterObserver('LastConnectionStatus', self)

		# Start trying to connect to the gateway.
		self._systemObject.ConnectToGateway()


	## User authentication button event.
	# @param self The object pointer.
	# @param event Unused.
	def OnAuthenticate(self, event):
		loginName = self._loginNameCtrl.GetValue()
		password = self._passwordCtrl.GetValue()

		event = Event(EventID_AuthenticateLogon,
			{'LoginNanme' : loginName, 'Password' : password})
		EventManager.Instance().QueueEvent(event)

		'''
		cProjectSelectWindow *frame;

		frame = new cProjectSelectWindow( NULL, wxID_ANY, wxEmptyString, 
										  wxDefaultPosition, wxSize( 350, 300 ) ); 

		# Show the newly created window - the frames, unlike simple controls, are
		# not shown when created initially)
		frame->Show( true );

		# Destroy this window, now it's been marked as closed!
		delete this;
		'''


	## Close dialog event.
	# @param self The object pointer.
	def OnClose(self, event):

		dlgResult = wx.MessageBox(MsgStr_CancelQuestion, MsgStr_CancelTitle,
			CancelMsgBoxState);
			
		if dlgResult == wx.YES:
					self._systemObject.ShutdownSystem()
					self.Destroy()


	##
	# Event when the timer ticks, the panels are updated.
	# @event Required, but not used.		
	def NotifyPropertyChange(self, property, value):
	
		# Update button state if connected/disconnected.
		if property == 'ConnectedToGateway':
			if value == True:
				self._logonBtn.Enable()
			else:
				self._logonBtn.Disable()

		# Update last connection status on logon dialog.
		elif property == 'LastConnectionStatus':
			self._statusCtrl.SetLabel(value)
						