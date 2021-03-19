#  Copyright 2014-2018 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////


## Base log system logger sink.
class LogSink(object):

	@property
	def Name(self):
		return self._name


	@property
	def LogLevels(self):
		return self._logLevels


	## LogSink class default constructor.
	def __init__(self, name, loglevels):
		self._name = name
		self._logLevels = loglevels

	
	## Log the message, taking in the log level, message string and time that 
	#  message was logged.
	def LogMsg(self, LogLevel, msg, timeStamp):
		raise NotImplementedError('LogMsg is not implemented')
