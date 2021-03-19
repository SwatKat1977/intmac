#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////


class ServiceEventType(object):
	ModeChange = 0
	UserDefined = 1

class ServiceEvent(object):
	@property
	def ID(self):
		return self.__eventID

	@property
	def Type(self):
		return self.__eventType

	@property
	def Data(self):
		return self.__eventData
		
	def __init__(self, eventID, eventType, eventData):
		self.__eventID = eventID
		self.__eventData = eventData	
		self.__eventType = eventType