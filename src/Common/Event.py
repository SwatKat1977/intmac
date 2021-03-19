#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////


# <summary>
# 
# </summary>
class Event(object):


	# <summary>
	# Property getter : Event ID
	# </summary>
	@property
	def ID(self):
		return self._eventID


	# <summary>
	# Property getter : Event body
	# </summary>
	@property
	def Body(self):
		return self._msgBody


	# <summary>
	# 
	# </summary>
	# <param name=""></param>
	def __init__(self, eventID, msgBody = None):
		self._eventID = eventID 
		self._msgBody = msgBody
