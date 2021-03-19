#  Copyright 2014-2018 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////


# Enumeration for log level.
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


# Log level as a human readable string.
LogLevelStr = {
 LogLevel.Emergency : "emerg",
 LogLevel.Alert : "alert",
 LogLevel.Critical : "crit",
 LogLevel.Error : "err",
 LogLevel.Warning : "warning",
 LogLevel.Notice : "notice",
 LogLevel.Info : "info", 
 LogLevel.Debug : "debug" }
