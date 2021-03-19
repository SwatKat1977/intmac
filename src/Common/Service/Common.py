#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////


class ServiceMode(object):
	Bootstrapping = 0
	OpeningNetworkConnections = 1
	StartingService = 2
	Open = 3
	ShuttingDown = 4


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
	Informational = 6

	# 7	Debug [debug] : Information useful for debugging.
	Debug = 7

LogLevelStr = {0 : "emerg", 1 : "alert", 2 : "crit", 3 : "err", 4 : "warning",
				5 : "notice", 6 : "info", 7 : "debug" }

ServiceModeStr = {0 : "Bootstrapping", 1 : "Opening Network Connections",
					2 : "Starting Service", 3 : "Open", 4 : "Shutting Down" }