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
	Handshaking = 0
	NotAuthenticated = 1
	Authenticating = 2
	Ready = 3


class ClientConnection(object):
	def __init__(self, conn, connID):
		self.connection = conn
		self.connectionID = connID
		self.connectionState = ConnectionState.Handshaking


class ClientInterfaceProtocol(LineReceiver, TimeoutMixin):

	# <summary>
	#
	# </summary>
	# <param name="factory"></param>
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
		print("[DEBUG] New client connections... Total : {0}"
		.format(self.factory.numberOfConns))
		
		# Create connection object for the new connection that has been made
		# and add it to the factory clients list.
		connObj = ClientConnection(self, self.connID)
		self.factory.clients[self.connID] = connObj


	# <summary>
	# An existing connection has been lost.
	# </summary>
	# <param name="reason">The reason for the disconnection</param>
	def connectionLost(self, reason):
		# Decrement number of client connections.
		self.factory.numberOfConns -= 1
		
		# Remove the client connection.
		del self.factory.clients[self.connID]
		
		print("[DEBUG] Client connections dropped... Total : {0}"
		.format(self.factory.numberOfConns))


	# <summary>
	# Line received callback.
	# </summary>
	# <param name="data">Data received</param>
	def lineReceived(self, data):
		defer = threads.deferToThread(
			self.factory.msgProcessor.ProcessMessage,
		 	data, self.factory.clients[self.connID])


class ClientInterfaceFactory(protocol.Factory):
	numberOfConns = 0
	clients = {}
	nextConnectionID = 0

	def __init__(self, msgProcessor):
		self.msgProcessor = msgProcessor
		nextConnectionID = 0


	def buildProtocol(self, addr):
		return ClientInterfaceProtocol(self)
