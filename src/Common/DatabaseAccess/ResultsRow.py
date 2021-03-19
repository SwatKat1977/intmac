#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////


# ***********************************************************************
# Definition of class to represent a row returned from the database.
# ***********************************************************************
class ResultsRow:

	##
	# Class constructor.
	def __init__(self):
		# List of columns (should be ResultColumn)
		self._columns = []
		self._currentItem = 0


	##
	# Get the number of columns in the row.
	# @return Number of columns
	def Size(self):
		return len(self._columns)


	##
	# Get a column based on the column ID (0 based index).  If the column ID
	# isn't valid then None is returned.
	# @columnID 0 index column ID
	# @return an instance of ResultColumn if valid, or None if invalid.
	def GetColumn(self, columnID):
		# if the column ID is invalid then return None.
		if columnID > (len(self._columns) - 1):
			return None

		# Return column instance.
		return self._columns[columnID]


	##
	# Get the first column, if there are no columns then None is returned,
	# otherwise return the first item (instance of ResultColumn).
	# @return an instance of ResultColumn if valid, or None if row is empty.
	def First(self):
		# If the row is empty then return None
		if len(self._columns) == 0:
			return None

		self._currentItem = 0
		return self._columns[self._currentItem]


	##
	# Get the next column, as long as it's not the last item, in which None is
	# returned.
	# @return an instance of ResultColumn if valid, or None if row is empty.
	def Next(self):
		colsLen = len(self._columns)

		# Check to make sure that:
		#  * There is at least one column and it's not at the end.
		if colsLen == 0 or self._currentItem == (columns.Count -1):
			return None

		self._currentItem += 1   
		return self._columns[self._currentItem]


	##	
	# Get the last column, as long as the row isn't empty, in which None is
	# returned.
	# @return an instance of ResultColumn if valid, or None if row is empty.
	def Last(self):
		if len(self._columns) == 0:
			return None

		self._currentItem = len(self._columns) - 1
		return self._columns[self._currentItem]


	##
	# Add a new column.
	def AddColumn(self, column):
		self._columns.append(column)
