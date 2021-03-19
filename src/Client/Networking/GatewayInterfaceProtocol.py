#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
from twisted.protocols.policies import TimeoutMixin
from twisted.protocols.basic import LineReceiver
from twisted.internet import reactor, threads
from twisted.internet.task import LoopingCall
from Common.Protocol.Client2Gateway import *
from Client.Networking.ConnectionState import ConnectionState
#from Client.SystemObject import SystemObject


# This needs to be set as a configurable item at sometime
KeepAliveInterval = 3


# <summary>
# Gateway interface network protocol.
# </summary>
class GatewayInterfaceProtocol(LineReceiver, TimeoutMixin):

	# <summary>
	# GatewayInterfaceProtocol default constructor.
	# </summary>
	# <param name="factory">Gateway interface protocol factory</param>
	# <param name="msgProcessor">Gateway interface message processor</param>
	def __init__(self, factory, msgProcessor, systemObject):
		self._factory = factory
		self._msgProcessor = msgProcessor
		self._keepAliveLoop = None
		self._connectionState = ConnectionState.Disconnected
		self._systemObject = systemObject


	# <summary>
	# Connection made event handler function.
	# </summary>
	def connectionMade(self):
		self._factory.connection = self

		print("[DEBUG] Connected...")
		self._systemObject.ConnectedToGateway = True
		self._systemObject.LastConnectionStatus = 'Connected to gateway'


		# Connection made triggers a state change as next step is to do a
		# handshake.
		self._connectionState = ConnectionState.Handshaking
		
		# Once the connection has been established, we then need to request a
		# handshake with the server, passing in the version of the protocol and
		# client.  Create params dictionary and pass to message builder.
		reactor.callFromThread(self._msgProcessor.SendMsg_RequestHandshake,
			connection=self)

		self._keepAliveLoop = LoopingCall(f=self._msgProcessor.SendKeepAliveMsg,
			connection=self)
		self._keepAliveLoop.start(KeepAliveInterval)


	# <summary>
	# Connection lost event handler function.
	# </summary>
	def connectionLost(self, reason):
		print("[DEBUG] Connection has been lost...")
		
		# Stop sending the 'keep alive' messages.
		self._keepAliveLoop.stop()


	# <summary>
	# Line received callback.
	# </summary>
	# <param name="data">Data received</param>
	def lineReceived(self, rawMessage):
		defer = threads.deferToThread(
			self._factory.msgProcessor.ProcessMessage,
		 	rawMessage, self._factory.connection)
