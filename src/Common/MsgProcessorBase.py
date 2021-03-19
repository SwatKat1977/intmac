#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
from Common.MessageProtocol import *


# <summary>
# Message Processor base class.
# </summary>
class MsgProcessorBase(object):

	# <summary>
	# MsgProcessorBase class default constructor.
	# </summary>
	def __init__(self):
		self._validMessages = {}
		self._msgProtocol = MessageProtocol()


	# <summary>
	# Process an incoming message, checking that it's valid before calling the
	# override function.
	# </summary>
	# <param name="rawMsg">Raw incoming message</param>
	# <param name="connection">Connection that message came from</param>
	def ProcessMessage(self, rawMsg, connection):
	
		msg = self._msgProtocol.UnpackMessage(rawMsg)
		
		if msg == None:
			return

		# Check to see if the message is valid (has ID tag and it's the right
		# type).
		if self.IsMessageValid(msg) == False:
			return False

		# Reset the timeout counter as data has been received.
		connection.resetTimeout()

		# Handle an incoming message.
		self.HandleIncomingMessage(msg, connection)


	# <summary>
	# Verify that the message is valid, firstly is the ID registered and an 
	# integer, secondly are the tags (if any) expected.
	# </summary>
	# <param name="rawMsg">Raw message (dictionary)</param>
	def IsMessageValid(self, msg):
		
		# Split header and body up.
		header = msg[0]
		body = msg[1]

		try:
			# Get the ID, 
			id = header.get(MsgTag_ID, None)

		# Message ID tag can't be found or format is invalid.
		except KeyError, ke:
			return False

		# Check if an ID is in the dictionary and is an integer.  If not then
		# return failed (False).
		if id == None or not isinstance(id, (int, long)):
			return False

		# Verify that the message ID is registered with the interface, return
		# error state (False) if it's not.
		if id not in self._validMessages:
			return False

		# Extract tags for the message.
		validTags = self._validMessages[id]
		
		# Verify each message tag, one at time.
		for tag in body.keys():
			# Check tag, return error if not in valid list.
			if tag not in validTags:
				return False

		# Everything was okay, return that (True).	
		return True


	# <summary>
	# Register a message ID and a list of it's valid tags.
	# </summary>
	# <param name="id">Message ID</param>
	# <param name="validTags">List of valid tags, default is empty list</param>
	def RegisterMessage(self, id, validTags = []):
		# Check that the message is valid.
		if id in self._validMessages:
			return False

		# Add the valid message ID.
		self._validMessages[id] = validTags


	# <summary>
	# ** Override ** function for handling incoming messages.
	# </summary>
	# <param name="rawMsg">Raw incoming message</param>
	# <param name="connection">Connection that message came from</param>
	def HandleIncomingMessage(self, rawMsg, connection):
		pass
