#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////


# ***********************************************************************
# Definition of class to represent a data set returned from the database.
# ***********************************************************************
class ResultsSet():
	
	##
	# Class constructor.
	def __init__(self):
		# List of rows of results.
		self._rows = []
		
		# Current row position.
		self._currentItem = 0


	##
	# Add a row to the set.
	# @newRow New row to add to the set
	def AddRow(self, newRow):
		self._rows.append(newRow)


	##
	# Get a row based on the row ID (0 based index).  If the row ID isn't valid
	# then None is returned.
	# @rowID 0 index row ID
	# @return an instance of ResultsRow if valid, or None if invalid.
	def GetRow(self, rowID):
		if rowID > len(self._rows):
			return None

		return self._rows[rowID]


	##
	# Get the number of rows in the set.
	# @return Number of rows
	def Size(self):
		return len(self._rows)


	##
	# Get the first row, if there are none then None is returned, otherwise
	# return the first row (instance of ResultsRow).
	# @return an instance of ResultsRow if valid, or None if it's empty.
	def First(self):
		if len(self._rows) == 0:
			return None

		self._currentItem = 0
		return self._rows[self._currentItem]


	##
	# Get the next row, as long as it's not the last one, in which None is
	# returned.
	# @return an instance of ResultsRow if valid, or None if set is empty.
	def Next(self):
		rowCount = len(self._rows)
		
		if (rowCount == 0) or (self._currentItem == (rowCount -1)):
			return None

		self._currentItem += 1
		return self._rows[self._currentItem]


	##	
	# Get the last row, as long as the set isn't empty, in which None is
	# returned.
	# @return an instance of ResultsRow if valid, or None if set is empty.
	def Last(self):
		if len(self._rows) == 0:
			return None

		self._currentItem = len(self._rows)  - 1
		return self._rows[self._currentItem]


	##	
	# Clear the contents of the set.
	def Clear(self):
		self._rows = []
		self._currentItem = 0		
