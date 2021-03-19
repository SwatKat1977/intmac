#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import threading
from twisted.internet import reactor
from twisted.internet.task import LoopingCall
from Common.Protocol.Client2Gateway import *
from Networking.GatewayInterfaceMsgProcessor import GatewayInterfaceMsgProcessor
from Networking.GatewayInterfaceFactory import GatewayInterfaceFactory
from Common.ObservableClass import ObservableClass
from Common.ObservableProperty import ObservableProperty


# Temporarily hard-code the server settings.
serverHost = 'localhost'
serverPort = 7000


class SystemObject(ObservableClass):

	# Event handler loop interval time (in seconds).
	EventHandlerLoopInterval = 2

	ConnectedToGateway = ObservableProperty('ConnectedToGateway')
	
	LastConnectionStatus = ObservableProperty('LastConnectionStatus')


	##  Property getter : SystemEventManager
	@property
	def SystemEventManager(self):
		return self._eventManager


	##  Property setter : SystemEventManager
	@SystemEventManager.setter
	def SystemEventManager(self, value):
		self._eventManager = value


	def __init__(self):
		super(SystemObject, self).__init__()

		self._logonDialog = None
		self._networkThread = None
		self._eventHandlingLoop = None

		self.ConnectedToGateway = False
		self.LastConnectionStatus = 'Unknown status...'

		# Netowork message processor object.
		self._msgProcessor = GatewayInterfaceMsgProcessor()

		self._interfaceFactory = GatewayInterfaceFactory(self._msgProcessor,
			self)

		# Register messages that the client supports.
		self.RegisterNetworkMessages()

		from Common.EventManager import EventManager
		self._eventHandlingLoop = LoopingCall(
			EventManager.Instance().ProcessNextEvent)	
		self._eventHandlingLoop.start(self.EventHandlerLoopInterval)


	## Register the network message ID's and tags that the client can be
	# expected to received.
	# @param self The object pointer.
	def RegisterNetworkMessages(self):
		# Register known message ID's.
		self._msgProcessor.RegisterMessage(
			Client2GatewayMsgID.HandshakeResponse, HandshakeResponseMsgs)
		self._msgProcessor.RegisterMessage(
			Client2GatewayMsgID.LogonResponse, LogonResponseMsgs)
		self._msgProcessor.RegisterMessage(
			Client2GatewayMsgID.ProjectsList, ProjectsListMsgs)
		self._msgProcessor.RegisterMessage(
			Client2GatewayMsgID.ProjectAssetsBulkData, ProjectAssetsBulkDataMsgs)


	# <summary>
	# </summary>
	# @param self The object pointer.
	def StartNetworkingSystem(self):
		# Starting the networking system runs in it's own thread.
		print("[INFO] Starting the network system")
		self._networkThread = threading.Thread(target=reactor.run, 
			kwargs={"installSignalHandlers": False})
		self._networkThread.start()


	# <summary>
	# </summary>
	# @param self The object pointer.
	def ShutdownSystem(self):
		# Stop the networking system runs in it's own thread.
		print("[INFO] Stopping the network system")

		if self._networkThread != None:
			# Shutdown the Twisted reactor and it's thread.
			reactor.callFromThread(reactor.stop)
			self._networkThread.join()


	# <summary>
	# </summary>
	# @param self The object pointer.
	def ConnectionToGatewayFailed(self, reason):
		print("[Error] Unable to connect to gateway retrying...")
		reactor.callLater(20, reactor.callFromThread, self.ConnectToGateway)
		self.LastConnectionStatus = 'Unable to connect to gateway retrying...'
		self.ConnectedToGateway = False


	# <summary>
	# </summary>
	# @param self The object pointer.
	def ConnectToGateway(self):
		from twisted.internet.endpoints import TCP4ClientEndpoint
		endpoint = TCP4ClientEndpoint(reactor, serverHost, serverPort)
		d = endpoint.connect(self._interfaceFactory)
		d.addErrback(self.ConnectionToGatewayFailed)