#  Copyright 2014-2018 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import datetime


# Application framework class.
class Application(object):

    ## IsRunning property (getter).
    #  @param self The object pointer.
	@property
	def IsRunning(self):
		return self._isRunning


    ## IsRunning property (setter).
    #  @param self The object pointer.
    #  @param value New value for property.
	@IsRunning.setter
	def IsRunning(self, value):
		self._isRunning = value


    ## Default constructor.
    #  @param self The object pointer.
	def __init__(self):
		self._isRunning = False


	## ** Overridable function **
	#  Start the application.
    #  @param self The object pointer.
	def Run(self):
		# Initialise the application, run main loop if initialised.
		if self._Initialise() == True:

			# Run the main loop.
			while self._isRunning == True:
				self._MainLoop()

		# Perform any shutdown required.
		self._Shutdown()


	## ** Overridable function **
    #  @param self The object pointer.
    #  @return True if initialise was successful, otherwise False.
	def _Initialise(self):
		return True


	## ** Overridable function **
    #  @param self The object pointer.
	def _MainLoop(self):
		pass


	## ** Overridable function **
    #  @param self The object pointer.
	def _Shutdown(self):
		pass
