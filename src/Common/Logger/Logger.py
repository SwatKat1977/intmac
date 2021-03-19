#  Copyright 2014-2018 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import datetime
from Singleton import Singleton


## Log level enumeration.
class LogLevel(object):
	# 0	Emergency [emerg] :	System is unusable.
	Emergency = 0

	# 1	Alert [alert] : Should be corrected immediately.
	Alert = 1
	
	# 2	Critical [crit] : Critical condition(s) in service.
	Critical = 2
	
	# 3	Error [err] : Error condition.
	Error = 3

	# 4	Warning	[warning] :	Indicates that an error will occur if action is not
	#			taken.
	Warning = 4

	#5	Notice	notice	Events that are unusual, but not error conditions.
	Notice = 5

	# 6	Informational [info] : Normal operational message that require no action.
	Info = 6

	# 7	Debug [debug] : Information useful for debugging.
	Debug = 7


# Log levels as human-readable abbreviation strings.
LogLevelStr = {
LogLevel.Emergency : "emerg",
LogLevel.Alert : "alert",
LogLevel.Critical : "crit",
LogLevel.Error : "err",
LogLevel.Warning : "warning",
LogLevel.Notice : "notice",
LogLevel.Info : "info",
LogLevel.Debug : "debug"
}


# List of all of the log level enumerations.
LogLevels = [LogLevel.Emergency, LogLevel.Alert, LogLevel.Critical,
	LogLevel.Error, LogLevel.Warning, LogLevel.Notice, LogLevel.Info,
	LogLevel.Debug]


# Definition that includes all log levels in a set.
AllLogLevels = set(LogLevels)


## Generic extensible logger class.
@Singleton
class Logger(object):

	## Generic extensible logger class default constructor.
	def __init__(self):
		self._logSinks = {}

		# Which log levels a log sink can process.  Create empty lists.
		self._logLevelToSink = {}
		for level in LogLevels:
			self._logLevelToSink[level] = []


	## Log and event.
	def Log(self, level, message, *args):
		
		# Create message if there arguments. (same format as string format)
		if args:
			message = message.format(*args)

		timestamp = datetime.datetime.now()

		for sink in self._logLevelToSink[level]:
				sink.LogMsg(level, message, timestamp)


	## Register a new sink.
	def RegisterSink(self, logSink):

		# Get the name of the log sink converted to upper-case.
		sinkName = logSink.Name.upper()

		# Make sure that we are not adding same log sink twice.
		if sinkName in self._logSinks.keys():
			return 'Sink name already exists'

		# Add to log sinks list.
		self._logSinks[sinkName] = logSink

		# For each acceptable log level listed in sink, add sink to that list
		# so if a log comes in with that level the logger will know how to
		# process it.
		for level in logSink.LogLevels:
			self._logLevelToSink[level].append(logSink)

		return ''


	## Unregister an existing sink.
	def UnregisteSink(self, name):

		# Always convert sink name to uppercase to make it case insensitive.
		sinkName = name.upper()

		# If the sink isn't know about then abandon unregister.
		if sinkName not in self._logSinks.keys():
			return

		# Move primary reference to the log sink.
		sinkInstance = self._logSinks[sinkName]
		del self._logSinks[sinkName]
	
		# Clear out the cross-references to individual log levels.
		for level in self._logLevelToSink:
			if sinkInstance in self._logLevelToSink[level]:
				self._logLevelToSink[level].remove(sinkInstance)

LoggerInst = Logger.Instance()
