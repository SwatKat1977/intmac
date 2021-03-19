#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
#from Gateway2AuthSrvMsgIds import *
#from GatewayInterface import *
#from ServerServices.AuthService.AuthService import *
#from ...AuthService import *

MsgTag_ID = 'ID'


class AuthInterfaceMsgProcessor(object):

	## TBD
	# @param self The object pointer.
	def __init__(self):
		self.validMessages = []


	## TBD
	# @param self The object pointer.
	def ProcessMessage(self, data, connection):
		# Check to see if the message is valid (has ID tag and it's the right
		# type).
		if self.IsMessageValid(data) == False:
			return False

		if data[MsgTag_ID] == MsgID_StartHandshake:
			self.ProccessStartHandshake(data, connection)
		
		elif data[MsgTag_ID] == MsgID_EndHandshake:
			self.ProccessEndHandshake(data, connection)


	## TBD
	# @param self The object pointer.
	def IsMessageValid(self, data):
		# Get the ID, 
		id = data.get(MsgTag_ID, None)

		# Check if an ID is in the dictionary and is an integer.  If not then
		# return failed (False).
		if id == None or not isinstance(id, (int, long)):
			print("[DEBUG] Missing or mistyped ID")
			return False

		if id not in self.validMessages:
			return False

		return True


	## Register a message.
	# @param self The object pointer.
	def RegisterMessageID(self, id):
		# Check that the message is valid.
		if id in self.validMessages:
			return False

		# Add the valid message ID.
		self.validMessages.append(id)


	## Process message 'ProccessStartHandshake'.
	# @param self The object pointer.
	# @param data Decompressed message.
	# @param conn Connection object.
	def ProccessStartHandshake(self, data, conn):
		# Check if the connection is in a valid state.
		if conn.connectionState > ConnectionState.Disconnected:
			return

		#conn.connection.transport.write(pickle.dumps({"ID" : 9001}))		
		conn.connectionState = ConnectionState.StartingHandshake


	## Process message 'ProccessEndHandshake'.
	# @param self The object pointer.
	# @param data Decompressed message.
	# @param conn Connection object.
	def ProccessEndHandshake(self, data, conn):
		# Check if the connection is in a valid state.
		if conn.connectionState != ConnectionState.StartingHandshake:
			return

		conn.connectionState = ConnectionState.Ready
		event = ServiceEvent(EventID_GatewayConnected,
			ServiceEventType.UserDefined, {})
		AuthService.Instance().ReceiveEvent(event)
