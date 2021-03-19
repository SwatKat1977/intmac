#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////


# ***********************************************************************
# Definition of class to represent a row returned from the database.
# ***********************************************************************
class ResultColumn:

	##
	# ResultColumn class constructor that takes the column name and value as
	# parameters.
	# @key Column name
	# @value Column value
	def __init__(self, key, value):
		self._key = key
		self._value = value


	# Get the column name.
	# @return the column name.
	def GetKey(self): return self._key


	##
	# Get the column value.
	# @return the column value.
	def GetValue(self): return self._value
