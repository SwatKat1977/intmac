#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
from twisted.internet import reactor
from Common.Protocol.Client2Gateway import *
from Common.MessageProtocol import *
from Client.Networking.ConnectionState import ConnectionState
from Common.MsgProcessorBase import MsgProcessorBase


## Class that implements a gateway interface message processor.
# It inherits from MsgProcessorBase.
class GatewayInterfaceMsgProcessor(MsgProcessorBase):

	## ClientInterfaceMsgProcessor default constructor.
	# @param self The object pointer.
	def __init__(self):
		MsgProcessorBase.__init__(self)
		self.validMessages = []
		self._messageProtocol = MessageProtocol()


	## Process message 'HandshakeResponse'.
	# @param self The object pointer.
	# @param rawMsg Decompressed message.
	# @param connection Connection object.
	def ProccessHandshakeResponse(self, rawMsg, connection):

		# =================================================
		# NOTE: Currently hard-coded to not check response.
		# =================================================
		# The handshake has been completed, but no user has been authenticated.
		connection._connectionState = ConnectionState.NotAuthenticated

		# NOTE: Currently hard-code a SendLogonCreds message and send it until
		# login is implemented on client.
		params = { MsgTag_SendLogonCreds_Username : 'admin',
			MsgTag_SendLogonCreds_Password : 'admin'}
		msg, errState = self._messageProtocol.BuildMessage(
			Client2GatewayMsgID.SendLogonCreds, params)
	
		# Verify message was built correctly, if it was then send it.
		if msg != None:
			self.SendToGateway(msg, connection)

		# Change state to 'User authentication in progress'
		connection._connectionState = ConnectionState.Authenticating


	## Process message 'LogonResponse'.
	# @param self The object pointer.
	# @param rawMsg Decompressed message.
	# @param connection Connection object.
	def ProccessLogonResponse(self, rawMsg, connection):
		# =================================================
		# NOTE: Currently hard-coded to not check response.
		# =================================================
		pass		


	## Process message 'ProjectsList'.
	# @param self The object pointer.
	# @param rawMsg Decompressed message.
	# @param connection Connection object.
	def ProccessProjectsList(self, rawMsg, connection):
		# =================================================
		# NOTE: NOT IMPLEMENTED YET.
		# =================================================
		domainNames = []
		projects = {}
		
		rawProjectsList = rawMsg[1][MsgTag_ProjectsList_Projects]
		for d in rawProjectsList.keys():
			domainNames.append(d)
			projects[d] = rawProjectsList[d]
			
		domStr = ", ".join(str(x) for x in domainNames)
		print("Domains  : {0}".format(domStr))
		print("Projects : {0}".format(projects))

		# Change state to 'User authenticated, awaiting project selection'
		connection._connectionState = ConnectionState.SelectingProject

		# NOTE: Currently hard-code a ProjectSelection message and send it
		# since the gui element isn't implemented yet on the client.
		params = { MsgTag_ProjectSelection_SelectedProject : 'a project'}
		msg, errState = self._messageProtocol.BuildMessage(
			Client2GatewayMsgID.ProjectSelection, params)

		# Verify message was built correctly, if it was then send it.
		if msg != None:
			self.SendToGateway(msg, connection)

	## Process message 'ProjectAssetsBulkData'.
	# @param self The object pointer.
	# @param rawMsg Decompressed message.
	# @param connection Connection object.
	def ProccessProjectAssetsBulkData(self, rawMsg, connection):
		print("[DEBUG] Handling Project Assets Bulk Data...")
		msgBody = rawMsg[1]
		print("[DEBUG] Msg body : {0}".format(msgBody))


	## Send 'Keep alive' message.
	# @param self The object pointer.
	# @param connection Connection object.
	def SendKeepAliveMsg(self, connection):
		msg, errState = self._messageProtocol.BuildMessage(
			Client2GatewayMsgID.KeepAlive)

		# Verify message was built correctly, if it was then send it.
		if msg != None:
			self.SendToGateway(msg, connection)


	## Process incoming messages that have already been unpacked ready for
	## parsing.
	# @param self The object pointer.
	# @param rawMsg Raw message.
	# @param connection Connection object.
	def HandleIncomingMessage(self, rawMsg, connection):
		msgID = rawMsg[0][MsgTag_ID]
		
		# 'Handshake Response' message.
		if msgID == Client2GatewayMsgID.HandshakeResponse:
			self.ProccessHandshakeResponse(rawMsg, connection)
		
		# 'Logon Response' message.
		elif msgID == Client2GatewayMsgID.LogonResponse:
			self.ProccessLogonResponse(rawMsg, connection)

		# 'Projects List' message.
		elif msgID == Client2GatewayMsgID.ProjectsList:
			self.ProccessProjectsList(rawMsg, connection)

		# 'Project Assets Bulk Data' message.
		elif msgID == Client2GatewayMsgID.ProjectAssetsBulkData:
			print 'ProjectAssetsBulkData'
			self.ProccessProjectAssetsBulkData(rawMsg, connection)


	## Send 'Request Handshake' message.
	# @param self The object pointer.
	# @param connection Connection object.
	def SendMsg_RequestHandshake(self, connection):

		# NOTE : Currently we will hard-code the protocol and client version.
		params = {
			MsgTag_RequestHandshake_Protocol : 0,
			MsgTag_RequestHandshake_Client : 0
			}
		msg, statusStr = self._messageProtocol.BuildMessage(
			Client2GatewayMsgID.RequestHandshake, params)

		# Verify message was built correctly, if it was then send it.
		if msg != None:
			self.SendToGateway(msg, connection)


	## Send the messages in the list to the gateway, if there is more than one
	## then they will be sent in order, but 1/2 a second part.
	# @param self The object pointer.
	# @param msgsList List of messages to send.
	# @param connection Connection object.
	def SendToGateway(self, msgsList, connection):
		callCount = 1

		for msg in msgsList:
			reactor.callLater(callCount * 0.5, connection.sendLine, msg)
			callCount +=1
