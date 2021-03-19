#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////

MsgTag_ID = 'ID'


class GatewayInterfaceMsgProcessor(object):
	
	def __init__(self):
		self.validMessages = []
		

	def ProcessMessage(self, data, connection):
		# Check to see if the message is valid (has ID tag and it's the right
		# type).
		if self.IsMessageValid(data) == False:
			return False


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


	def RegisterMessageID(self, id):
		# Check that the message is valid.
		if id in self.validMessages:
			return False

		# Add the valid message ID.
		self.validMessages.append(id)
