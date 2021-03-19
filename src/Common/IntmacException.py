#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////


# ***********************************************************************
# Definition of Intmac Exception class
# ***********************************************************************
class IntmacException(Exception):
	def __init__(self, ID, message = ""):

		# Call the base class constructor with the parameters it needs
		super(IntmacException, self).__init__(message)

		self.exceptionID = ID
