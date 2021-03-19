#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////


import datetime
from Common import *
from ServiceEvent import *


class ServiceBase(object):
	@property
	def ServiceMode(self):
		return self.__serviceMode


	@ServiceMode.setter
	def ServiceMode(self, value):
		self.__serviceMode = value


	@property
	def ServiceRunning(self):
		return self.__running


	@ServiceMode.setter
	def ServiceRunning(self, value):
		self.__running = value


	def __init__(self):
		self.__serviceMode = ServiceMode.Bootstrapping
		self.__running = False


	def ReceiveEvent(self, event):
		if event.Type == ServiceEventType.ModeChange:
			if not isinstance(event.Data, (int, long)):
				raise ValueError("Mode change event : Mode not an integer")

			# Verify that the service mode is valid (not outside the value of
			# the enumeration), if it is then raise an exception.
			if event.Data < ServiceMode.Bootstrapping \
				or event.Data > ServiceMode.ShuttingDown:
				raise ValueError("Invalid service mode ({0})".format(event.Data))

			self.HandleModeChange(event)

		elif event.Type == ServiceEventType.UserDefined:
			self.HandleCustomEvent(event)
	
		else:
			# Unknown event type - ignore
			pass


	# <summary>
	# ** Overridable function **
	# </summary>
	def Run(self):
		# Initialise the service, run main loop if initialised.
		if self.Initialise() == True:
			# Run the main loop.
			while self.__running == True:
				self.MainLoop()

		self.Shutdown()

		
	# <summary>
	# ** Overridable function **
	# </summary>
	def LogEvent(self, level, message, *args):
		# Date string in format YYYY/MM/DD hh:mm:ss (e.g. 2017/01/27 20:48:09)
		today = datetime.date.today()
		dateStr = datetime.datetime.now().strftime("%Y/%m/%d %H:%M:%S")
		
		# Create message if there arguments. (same format as string format)
		if args:
			message = message.format(*args)

		print("{0} [{1}] {2}".format(dateStr, LogLevelStr[level], message))


	# <summary>
	# ** Overridable function **
	# </summary>
	def Initialise(self):
		return True


	# <summary>
	# ** Overridable function **
	# </summary>
	def MainLoop(self):
		pass


	# <summary>
	# ** Overridable function **
	# </summary>
	def Shutdown(self):
		pass


	# <summary>
	# ** Overridable function **
	# Handle service mode changes, by default it will just set the new mode
	# that comes from the event passed in.  Additional functionality could be
	# added in a derived class.
	# </summary>
	# <param name="event">Service event object</param>
	def HandleModeChange(self, event):
		self.serviceMode = event.Data


	# <summary>
	# ** Overridable function **
	# Handle custom service events.  By default it doesn't process them.
	# Additional functionality could be added in a derived class.
	# </summary>
	# <param name="event">Service event object</param>
	def HandleCustomEvent(self, event):
		pass


	# <summary>
	# ** Overridable function **
	# </summary>
	def MainLoop(self):
		pass

	# <summary>
	# ** Overridable function **
	# </summary>
	def Shutdown(self):
		pass
