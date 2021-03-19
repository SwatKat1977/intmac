#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
from __future__ import division
import json
import numbers

# Maximum size (default) is 16384, make message size only 15500.
MaximumMsgSize = 15500

# Header Tags
MsgTag_ID = 'MsgID'
MsgTag_MsgParts = 'MsgParts'
MsgTag_MsgPart = 'MsgPart'


# <summary>
# Message builder class.
# </summary>
class MessageProtocol(object):

	# <summary>
	# Build a message, ready for transmission.  If the message ID is invalid
	# then the tuple of (None, <error message>) is returned.
	# </summary>
	# <param name="id">Message ID</param>
	# <param name="msgBody">Message body, default is {}, which represents
	# no parameters.  Type has to be dictionary.</param>
	def BuildMessage(self, id, msgBody = {}):
		msgs = []

		# Check that the message ID is an integer, anything else is invalid.
		if not isinstance(id, numbers.Number):
			return (None, 'message ID is not valid type (integer)')

		fullMsgBody = json.dumps(msgBody)
		msgBodyLength = len(fullMsgBody)
		
		# Calculate total message parts, if it's too big than MaximumMsgSize
		# then divide size, round up using math.ceil and convert to integer.
		totalParts = 1 if msgBodyLength <= MaximumMsgSize else \
			int(math.ceil(msgBodyLength / MaximumMsgSize))

		startPoint = 0
		msgBodyPos = 0

		for partNo in range(totalParts):
			# Calculate where abouts in msg to stop.  This could further than
			# the end of the message, but that doesn't matter.
			msgBodyPos = (partNo + 1) * MaximumMsgSize
			
			# Build message header.
			header = {MsgTag_ID : id, MsgTag_MsgParts : totalParts,
				MsgTag_MsgPart : partNo }

			# Build final message.
			msg = (header, fullMsgBody[startPoint:msgBodyPos])

			# Create json to data-interchange format and return it.
			jsonMsg = json.dumps(msg)		

			# Add message to list.
			msgs.append(jsonMsg)

			# Move start point to where last we just finished.
			startPoint = msgBodyPos

		return (msgs, '')


	# <summary>
	# Attempt to unpack a message. If the message is invalid
	# then None is returned, otherwise return unpacked msg.
	# </summary>
	# <param name="msg">Packed message</param>
	def UnpackMessage(self, msg):
		# Unpack JSON encoded message.
		try:
			unpackedMsg = json.loads(msg)
		except (ValueError, TypeError):
			return None

		# Unpack JSON encoded message body and rebuild message.
		try:
			unpackedBody = json.loads(unpackedMsg[1])
			return [unpackedMsg[0], unpackedBody]
		except (ValueError, TypeError):
			return None
		

	# <summary>
	# TEMPORARY until protocols matches rest and use base class.
	# </summary>
	# <param name=""></param>
	def IsMessageFormatValid(self, msg):
		try:
			# Get the ID, 
			id = msg[0].get(MsgTag_ID, None)

		# Message ID tag can't be found or format is invalid.
		except KeyError:
			return False

		# Check if an ID is in the dictionary and is an integer.  If not then
		# return failed (False).
		if id == None or not isinstance(id, (int, long)):
			return False

		return True
