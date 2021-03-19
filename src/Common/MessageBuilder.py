#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import json
import numbers

MsgTag_ID = 'ID'
MsgTag_Type = 'MsgType'


# <summary>
# Message type enumeration.
# </summary>
class MessageType(object):
	# System messages, e.g. handshaking.
	System = 0

	# Authentication messages, e.g. login and logout.
	Authentication = 1

	# Request for data, for example request details of test.
	DataRequest = 2


# <summary>
# Message builder class.
# </summary>
class MessageBuilder(object):

	# <summary>
	# Build a message, ready for transmission.  If the message ID is invalid
	# then the tuple of (None, <error message>) is returned.
	# </summary>
	# <param name="id">Message ID</param>
	# <param name="messageType">Message type</param>
	# <param name="params">Message parameters, default is None, which
	# represents no parameters.  Type has to be dictionary.</param>
	def BuildMessage(self, id, messageType, params = None):
		msg = {}

		# Check that the message ID is an integer, anything else is invalid.
		if not isinstance(id, numbers.Number):
			return (None, 'message ID is not valid type (integer)')

		# Add ID to the message.
		msg[MsgTag_ID] = id

		# Add message type to the message
		msg[MsgTag_Type] = messageType

		# Add parameters if it's not None and type dictionary.
		if not params == None and isinstance(params, dict):
			for key in params.keys():
				msg[key] = params[key]

		# Create json to data-interchange format and return it.
		jsonMsg = json.dumps(msg)		
		return (jsonMsg, '')
