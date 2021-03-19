#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import wx
from Common.Singleton import *
from SystemObject import SystemObject
		
'''
from HCI.MainDialog.MainDialog import *
'''


## Intmac top-level client class (derived from wx.App).
class Client(wx.App):

	## Default class constructor.
	# @param self The object pointer.
	def OnInit(self):
		# Create instance of the system object.
		self._systemObject = SystemObject()

		#print self._systemObject.SystemEventManager
		#sysObj.ISystemEventManager = None

		# Start the networking system and thread.
		self._systemObject.StartNetworkingSystem()

		'''
		#  Temporary
		if self._commsLayer.ConnectToServer(serverInfo) == False:
			wx.MessageBox("Unable to connect to server..."
				, "Connection Error", wx.OK | wx.ICON_ERROR)
			self.OnExit()
			return True

		# Start the comms thread and then check it's actually running.
		self._commsThread.start()
		if self._commsThread.is_alive() == False:
			wx.MessageBox("Unable start comms thread, aborting..."
                    , "FATAL Error"
                    , wx.OK | wx.ICON_ERROR)
			self.OnExit()
		'''

		"""
		bool hiddenAuthLoginOK = false;

		while ( hiddenAuthLoginOK == false )
		{
			if ( m_stateModel->GetProjectsList().m_listLoaded == true )
			{
				hiddenAuthLoginOK = true;
			}

			Sleep( 10 );
		}

		MessageProtocol::MessageID currentMsgID = m_msgHandler->GetCurrentMessage();

		while ( currentMsgID < MessageProtocol::MsgID_ProjectsListBulkData )
		{
			Sleep( 10 );
			currentMsgID = m_msgHandler->GetCurrentMessage();
		}
		"""

		from HCI.LogonDialog import LogonDialog
		lg = LogonDialog(self, self._systemObject)
		lg.Show(True)
		
		# ====----=======--TEMPORARY--===========------
		#mainDialog = MainDialog('projectModel', 'stateModel', 'eventHandler')
		#mainDialog.Show(True)
		# ====----=======----===========------

		return True
