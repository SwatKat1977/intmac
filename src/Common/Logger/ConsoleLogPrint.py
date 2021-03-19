#  Copyright 2014-2018 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
from LogSink import *
from Logger import *


class ConsoleLogPrint(LogSink):

	## <description goes here...>
	def __init__(self, name, loglevels):
		super(ConsoleLogPrint, self).__init__(name, loglevels)

	## <description goes here...>
	def LogMsg(self, LogLevel, msg, timeStamp):
		dateStr = timeStamp.strftime("%Y/%m/%d %H:%M:%S")

		print("{0} [{1}] {2}".format(dateStr, LogLevelStr[LogLevel], msg))
