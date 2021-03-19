#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
from Common.Singleton import *


class EventManagerStatusCode(object):
	Success = 0
	InvalidEventID = 1
	EventManagerDisabled = 2


@Singleton
class EventManager(object):

	# <summary>
	# 
	# </summary>
	def __init__(self):
		self._enabled = True
		self._eventHandlers = {}
		self._events = []


	# <summary>
	# Queue a new event
	# </summary>
	# <param name=""></param>
	# <returns>Return codes:
	#	EventManagerStatusCode.Success
	#	EventManagerStatusCode.InvalidEventID
	# </returns>
	def QueueEvent(self, event):
		# Return status from the QueueEvent function
		returnStatus = EventManagerStatusCode.Success

		# Only queue the event, if event manager is enabled (running)
		if self._enabled == False:
			return EventManagerStatusCode.EventManagerDisabled

		# Add event to queue.  Validate that the event is known about, if it is
		# then add it to the event queue for processing otherwise return
		# unknown status.
		if self.IsValidEventType(event.ID) == False:
			return EventManagerStatusCode.InvalidEventID
	
		# Add the event into the queue.
		self._events.append(event)

		# Return 'success' status.
		return EventManagerStatusCode.Success


	# <summary>
	# Register an event with the event manager
	# </summary>
	# <param name="callback">Event callback function</param>
	def RegisterEvent(self, eventID, callback):
		if eventID in self._eventHandlers:
			retrun

		self._eventHandlers[eventID] = callback
		print self._eventHandlers
 
 
 	# <summary>
	# Process the next event, if any exists.  An error will be generated
	# if the event ID is invalid (should never happen).
	# </summary>
	# <returns>Return codes:
	#	EventManagerStatusCode.Success
	#	EventManagerStatusCode.InvalidEventID
	# </returns>
	def ProcessNextEvent(self):
		# If nothing is ready for processing, return 0 (success)
		if len(self._events) == 0:
			return EventManagerStatusCode.Success

		#  Get the first event from the list. 
		event = self._events[0]

		# Check to see event ID is valid, if an unknown event ID then return
		# the 'invalid event id' error.
		if event.ID not in self._eventHandlers:
			return EventManagerStatusCode.InvalidEventID

		#  Call the event processing function, this is defined by the
		#  registered callback function.
		self._eventHandlers[event.ID](event)

		#  Once the event has been handled, delete it.. The event handler
		# function should deal with issues with the event and therefore
		#  deleting should be safe.
		self._events.pop(0)

		# Return 'success' status.
		return EventManagerStatusCode.Success;


	# <summary>
	# Delete all events.
	# </summary>
	def DeleteAllEvents(self):
		del self._events[:] 


	# <summary>
	# Check if an event is valid.
	# </summary>
	# <param name="eventID">Event ID to validate.</param>
	# <returns>Return codes: Valid = True.  Invalid = False# </returns>
	def IsValidEventType(self, eventID):
		return eventID in self._eventHandlers
