#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
from time import *
from Common.RawSocket.IPV4SocketLayer import IPV4SocketLayer 
from Common.Messaging.MessageHandler import *
from Common.Messaging.NetConnection import *


ConnectTimeoutSecs = 1
ConnectionTimeoutSecs = 10


# ***********************************************************************
# Definition of Raw IPV4 Server Information class
# ***********************************************************************
class RawIPV4ServerInfo:

	def __init__(self):
		self._serverHost = None
		self._serverPort = None


# ***********************************************************************
# Definition of Raw IPV4 Comms System class
# ***********************************************************************
class RawIPV4CommsSystem:

 	##
	# Class constructor.
	def __init__(self, messageHandler):
		self._socketLayer = None
		self._clientSocket = None
		self._msgHandler = messageHandler
		self._connInfo = None
	
		# Create socket that client will communicate through.
		self._socketLayer = IPV4SocketLayer()
		self._clientSocket = self._socketLayer.CreateSocket()
		self._clientSocket.SetBlocking(False)

		# Create connection info class.
		self._connInfo = NetConnection(self._clientSocket)
	
		self._RegisterMessages()


 	##
	# Class destructor.
	def __del__(self):
		del self._socketLayer
		del self._clientSocket
		del self._msgHandler
		del self._connInfo


 	##
	# Register any messages that are acceptable to accept.
	def _RegisterMessages(self):
		"""
		self._msgHandler._RegisterMessage(MsgDef_HandshakeResponse)
		self._msgHandler._RegisterMessage(MsgDef_AuthenticationStatus)
		self._msgHandler._RegisterMessage(MsgDef_BeginProjectsBulkImport)
		self._msgHandler._RegisterMessage(MsgDef_ProjectsListBulkData)
		self._msgHandler._RegisterMessage(MsgDef_ProjectSelectionStatus)
		self._msgHandler._RegisterMessage(MsgDef_BeginProjectAssetsBulkData)
		self._msgHandler._RegisterMessage(MsgDef_ProjectAssetsBulkData)
		"""
		pass


 	##
	# Try to connect the server using the credentials supplied within the 
	# serverInfo object.  As this is a socket connection it could take
	# multiple passes to complete the handshake.
	# @param serverInfo Object containing server connection info.
	# @return True if successful, false on failure.
	def ConnectToServer(self, serverInfo):
		# If already connected, don't try again.
		if self.IsConnected() == True: return True

		# Attempt connection, if it's successful return true
		connStatus = self._clientSocket.Connect(serverInfo._serverHost,
			serverInfo._serverPort, ConnectTimeoutSecs)

		if connStatus == True:
			# Update the last message timestamp
			self._connInfo._lastMsgTimestamp = time()
			return True
		
		if connStatus == False and self._clientSocket.incompleteConnect == False:
			self._clientSocket.Close()
			self._clientSocket = self._socketLayer.CreateSocket()
			self._clientSocket.SetBlocking(False)
	
		return False


 	##
	# Disconnect from the server.
	def DisconnectFromServer(self):
		self._clientSocket.Close()


 	##
	# Get the connected status.
	# @return True if connected, False if not connected.
	def IsConnected(self):
		return self._clientSocket.isConnected


 	##
	# Manage the connection to the server, this involves checking when the last
	# message was processed exceeds the timeout amount (in seconds).
	def ManageConnectionToServer(self):
		# Only attempt to connect if it's not already connected.
		if self.IsConnected() == True:
		
			# Get the current time (in seconds)
			currentTime = time()
	
			#if the connection has timed out then perform a disconnect.
			if currentTime - self._connInfo._lastMsgTimestamp >= ConnectionTimeoutSecs:
				self.DisconnectFromServer();
