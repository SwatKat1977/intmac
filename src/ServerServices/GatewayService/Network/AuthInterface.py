#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import json
import time
import threading
from twisted.internet import protocol, reactor, threads, task
from twisted.internet.defer import Deferred
from twisted.internet.task import LoopingCall
from twisted.protocols.basic import LineReceiver
from Protocol.Gateway2AuthIDs import *
from Service.Common import LogLevel
from GatewayServiceApp import *
from ServiceEvents import *


# This needs to be set as a configurable item at sometime
KeepAliveInterval = 3


# <summary>
# Authentication service (Twisted) network protocol.
# </summary>
class AuthInterfaceProtocol(LineReceiver):

	# <summary>
	# Class constructor.
	# </summary>
	# <param name="app">Instance of service application</param>
	def __init__(self, app):
		self._app = app
		self._keepAliveLoop = None


	# <summary>
	# Send 'Keep alive' message.
	# </summary>
	def SendKeepAliveMsg(self):
		msg = {"ID" : MsgID_KeepAlive}
		self.sendLine(json.dumps(msg))


	# <summary>
	# Send 'Start Handshake' message.
	# </summary>
	def SendMsg_StartHandshake(self):
		msg = {"ID" : MsgID_StartHandshake}
		self.sendLine(json.dumps(msg))


	# <summary>
	# Send 'End Handshake' message.
	# </summary>
	def SendMsg_EndHandshake(self):
		msg = {"ID" : MsgID_EndHandshake}
		self.sendLine(json.dumps(msg))


	# <summary>
	# Twisted event 'connectionMade'.
	# </summary>
	def connectionMade(self):
		self._app.LogEvent(LogLevel.Informational,
			"Connection to Authentication Service established...")

		# Send the 'start handshake' message.
		reactor.callFromThread(self.SendMsg_StartHandshake) 

		# Send the 'end handshake' message as currently nothing else to do.
		reactor.callFromThread(self.SendMsg_EndHandshake) 

		event = ServiceEvent(EventID_AuthServiceConnected,
			ServiceEventType.UserDefined, {})
		self._app.ReceiveEvent(event)

		self._keepAliveLoop = LoopingCall(self.SendKeepAliveMsg)
		self._keepAliveLoop.start(KeepAliveInterval)


	# <summary>
	# An existing connection has been lost.
	# </summary>
	# <param name="reason">The reason for the disconnection</param>
	def connectionLost(self, reason):
		self._app.LogEvent(LogLevel.Informational,
			"Connection to Authentication Service lost...")
		self._keepAliveLoop.stop()

		# Attempt to re-connect to the authentication service.
		reactor.callFromThread(self._app.ConnectToAuthService) 


	def dataReceived(self, data):
		try:
			unpickledData = json.loads(data)
		except:
			return


# <summary>
# Authentication service (Twisted) network protocol factory.
# </summary>
class AuthInterfaceFactory(protocol.ClientFactory):

	# <summary>
	# Class constructor.
	# </summary>
	def __init__(self, msgProcessor, app):
		self._msgProcessor = msgProcessor
		self._app = app


	# <summary>
	# Create an instance of a subclass of Protocol.
	# </summary>
	def buildProtocol(self, addr):
		return AuthInterfaceProtocol(self._app)
