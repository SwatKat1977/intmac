#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import json
from twisted.internet import protocol, reactor, threads #, ssl
from twisted.protocols.policies import TimeoutMixin
from twisted.protocols.basic import LineReceiver


class ConnectionState(object):
	Disconnected = 0
	StartingHandshake = 1
	Ready = 2


class Connection(object):
	def __init__(self, conn, connID):
		self.connection = conn
		self.connectionID = connID
		self.connectionState = ConnectionState.Disconnected


class GatewayInterfaceProtocol(LineReceiver, TimeoutMixin):
	def __init__(self, factory):
		self.factory = factory
		self.connID = -1

	# <summary>
	# A new connection has been established.
	# </summary>
	def connectionMade(self):
		# Set timeout for a connection to be 5 seconds
		self.setTimeout(10)
		
		# New connection, increment the number of connections.
		self.factory.numberOfConns += 1
		
		# New connection, increment next connection ID
		self.factory.nextConnectionID += 1
		
		# Set connection ID for this object.
		self.connID = self.factory.nextConnectionID
		
		# Create connection object for the new connection that has been made
		# and add it to the factory clients list.
		connObj = Connection(self, self.connID)
		self.factory.gateways[self.connID] = connObj


	# <summary>
	# An existing connection has been lost.
	# </summary>
	# <param name="reason">The reason for the disconnection</param>
	def connectionLost(self, reason):
		from ..AuthService import *
		from Common.Service.ServiceEvent import *
		from ..ServiceEvents import *

		self.factory.numberOfConns -= 1
		del self.factory.gateways[self.connID]
		event = ServiceEvent(EventID_GatewayDisconnected,
			ServiceEventType.UserDefined, {})
		AuthService.Instance().ReceiveEvent(event)


	# <summary>
	# Line received callback.
	# </summary>
	# <param name="data">Data received</param>
	def lineReceived(self, data):
		# Reset the timeout counter as data has been received.
		self.resetTimeout()

		try:
			msg = json.loads(data)

		# Unpickle failure exception caught.
		except Exception, e:
			return
			
		defer = threads.deferToThread(
			self.factory.msgProcessor.ProcessMessage,
		 	msg, self.factory.gateways[self.connID])


class GatewayInterfaceFactory(protocol.Factory):
	numberOfConns = 0
	gateways = {}
	nextConnectionID = 0

	def __init__(self, msgProcessor):
		self.msgProcessor = msgProcessor
		nextConnectionID = 0


	def buildProtocol(self, addr):
		return GatewayInterfaceProtocol(self)

