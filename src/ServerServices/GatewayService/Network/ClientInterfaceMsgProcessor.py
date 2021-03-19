#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
from twisted.internet import reactor
from Protocol.Client2Gateway import *
from MessageProtocol import *
from CommonDefinitions import AssetType


# <summary>
# Class that implements a client interface message processor.
# </summary>
class ClientInterfaceMsgProcessor(object):

	## ClientInterfaceMsgProcessor default constructor.
	# @param self The object pointer.
	def __init__(self):
		self.validMessages = []
		self._msgProtocol = MessageProtocol()


	## Process message 'RequestHandshake'.
	# @param self The object pointer.
	# @param rawMsg Decompressed message.
	# @param connection Connection object.
	def ProccessRequestHandshake(self, rawMsg, connection):
		# Response to handshake request.
		# NOTE: Currently hard-coded to 1 (OK),  the other values tbd.
		params = { MsgTag_HandshakeResponse_ResponseStatus : 1 }

		# Build the response message and send it.
		msg, errState = self._msgProtocol.BuildMessage(
			Client2GatewayMsgID.HandshakeResponse, params)

		# Verify message was built correctly, if it was then send it.
		if msg != None:
			self.SendMsgsToClient(msg, connection)


	## Process message 'SendLogonCreds'.
	# @param self The object pointer.
	# @param rawMsg Decompressed message.
	# @param connection Connection object.
	def ProccessSendLogonCreds(self, rawMsg, connection):

		# =================================================
		# NOTE: Currently hard-coded to not check response.
		# =================================================

		# Response to logon authentication request.
		# NOTE: Currently hard-coded to 1 (OK),  the other values tbd.
		params = { MsgTag_LogonResponse_ResponseStatus : 1 }

		# Build the response message and send it.
		msg, errState = self._msgProtocol.BuildMessage(
			Client2GatewayMsgID.LogonResponse, params)

		# Verify message was built correctly, if it was then send it.
		if msg != None:	
			self.SendMsgsToClient(msg, connection)

		# =================================================
		# NOTE: Currently hard-coded project details
		# =================================================
		# List of projects message.
		# NOTE: Currently hard-coded to 1 (OK),  the other values tbd.
		#projects = (("TestDomainA", "TestProjectA"), ("TestDomainB", "TestProjectB"))		
		projects = {"TestDomainA" : ["TestProjectA_01", "TestProjectA_02"],
			"TestDomainB" : ["TestProjectB"]}
	
		params = { MsgTag_ProjectsList_Projects : projects }

		# Build the response message and send it.
		msg, errState = self._msgProtocol.BuildMessage(
			Client2GatewayMsgID.ProjectsList, params)

		# Verify message was built correctly, if it was then send it.
		if msg != None:	
			self.SendMsgsToClient(msg, connection)


	## Process message 'ProjectSelection'.
	# @param self The object pointer.
	# @param rawMsg Decompressed message.
	# @param connection Connection object.
	def ProccessProjectSelection(self, rawMsg, connection):
		print "[DEBUG] a project has been selected"

		# =================================================
		# NOTE: Currently hard-coded project assets.
		# =================================================
		
		# asset id | parent ID | name | type
		assets = (
                [1, 0, 'Test Filter 01', AssetType.Filter],
                [2, 1, 'Test Item 01', AssetType.TestItem],
                [3, 1, 'Test Item 02', AssetType.TestItem],
                [4, 1, 'Test Item 03', AssetType.TestItem],
                [5, 1, 'Test Item 04', AssetType.TestItem],
                [6, 1, 'Test Item 05', AssetType.TestItem],
                [7, 1, 'Test Item 06', AssetType.TestItem],
                [8, 1, 'Test Item 07', AssetType.TestItem],
                [9, 1, 'Test Item 08', AssetType.TestItem],
                [10, 1, 'Test Item 09', AssetType.TestItem],
                [11, 1, 'Test Item 10', AssetType.TestItem],
                [12, 1, 'Test Item 11', AssetType.TestItem],
                [13, 1, 'Test Item 11', AssetType.TestItem],
                [14, 1, 'Test Item 12', AssetType.TestItem],
                [15, 1, 'Test Item 13', AssetType.TestItem],
                [16, 1, 'Test Item 14', AssetType.TestItem],
                [17, 1, 'Test Item 15', AssetType.TestItem],
                [18, 1, 'Test Item 16', AssetType.TestItem],
        )

		params = { MsgTag_ProjectAssetsBulkData_Assets : assets }

		# Build the response message and send it.
		msgsList, errState = self._msgProtocol.BuildMessage(
			Client2GatewayMsgID.ProjectAssetsBulkData, params)

		# Verify message was built correctly, if it was then send it.
		if msgsList != None:
			self.SendMsgsToClient(msgsList, connection)
		

		'''
		std::string releasesListXML = 
		"<releases> \
			 <release id=\"1\" parentID=\"0\" name=\"Server-Side\" type=\"folder\"/> \
			 <release id=\"2\" parentID=\"0\" name=\"Client-Side\" type=\"folder\"/> \
			 <release id=\"3\" parentID=\"2\" name=\"Client\" type=\"folder\"/> \
			 <release id=\"4\" parentID=\"3\" name=\"V0.1.0\" type=\"release\"/> \
			 <release id=\"5\" parentID=\"3\" name=\"V0.2.0\" type=\"release\"/> \
			 <release id=\"6\" parentID=\"5\" name=\"Cycle 1\" type=\"cycle\"/> \
			 <release id=\"7\" parentID=\"5\" name=\"Cycle 2\" type=\"cycle\"/> \
			 <release id=\"8\" parentID=\"1\" name=\"Server\" type=\"release\"/> \
			 <release id=\"9\" parentID=\"1\" name=\"Server Emulator\" type=\"release\"/> \
		</releases>";
		const int releasesListXMLSize = releasesListXML.size();
		'''


	## Process incoming message.
	# @param self The object pointer.
	# @param data Decompressed message.
	# @param connection Connection object.
	def ProcessMessage(self, data, connection):
	
		# Attempt to unpack the message if it's valid (not valid!).
		msg = self._msgProtocol.UnpackMessage(data)

		if msg == None:
			return

		# Check to see if the message is valid (has ID tag and it's the right
		# type).
		if self.IsMessageValid(msg) == False:
			return False
		
		# Reset the timeout counter as data has been received and format of it
		# is good.
		connection.connection.resetTimeout()
		
		msgID = msg[0][MsgTag_ID]

		# 'Request Handshake' message.
		if msgID == Client2GatewayMsgID.RequestHandshake:
			self.ProccessRequestHandshake(msg, connection)

		elif msgID == Client2GatewayMsgID.SendLogonCreds:
			self.ProccessSendLogonCreds(msg, connection)
			
		elif msgID == Client2GatewayMsgID.ProjectSelection:
			self.ProccessProjectSelection(msg, connection)


	def IsMessageValid(self, msg):
		if self._msgProtocol.IsMessageFormatValid(msg) == False:
			return False

		# Split message into header and body
		header, body = msg
		
		# Get the ID, 
		id = header.get(MsgTag_ID, None)

		if id not in self.validMessages:
			return False

		return True


	def RegisterMessageID(self, id):
		# Check that the message is valid.
		if id in self.validMessages:
			return False

		# Add the valid message ID.
		self.validMessages.append(id)


	# <summary>
	# 
	# </summary>
	# <param name="msgsList">List of messages to send</param>
	def SendMsgsToClient(self, msgsList, connection):
		callCount = 1

		for msg in msgsList:
			reactor.callLater(callCount * 1,
				connection.connection.sendLine, msg)
			
			callCount +=1
