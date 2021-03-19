#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import mysql.connector
from mysql.connector import errorcode
from ResultColumn import *
from ResultsSet import *
from ResultsRow import *


# ***********************************************************************
# Definition of class to represent a connection to a MySQL database.
# ***********************************************************************
class MySQLConnection:

	##
	# MySQLConnection class constructor, passing in the connection information.
	# @server MySQL hostname/IP address
	# @username MySQL username
	# @password Optional argument to retrieve Last row ID if INSERT.
	# @database Name of the database to connect to.
	# If the connection fails then an ValueError exception is raised with a
	# description of the fault.
	def __init__(self, server, username, password, database):
		self._isConnected = False
		self._dbObject = None

		config ={
			'user': username,
			'password': password,
			'host': server,
			'database': database
			}

		try:
			self._dbObject = mysql.connector.connect(**config)
		
		except mysql.connector.Error as err:
			errStr = err
			
			if err.errno == errorcode.CR_CONN_HOST_ERROR:
				errStr = "Unable to connect to MySQL host"
			elif err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
				errStr = "Invalid username / password"
			elif err.errno == errorcode.ER_BAD_DB_ERROR:
				errStr = "Database does not exist"
			
			raise ValueError(errStr)

		self._isConnected = True


	##
	# MySQLConnection class destructor, which will close the connection if it
	# is open.
	# @exc_type Unused
	# @exc_val Unused
	# @exc_tb Unused
	def __exit__(self, exc_type, exc_val, exc_tb):
		self.CloseConnection()


	##
	# Get status of the database connection.
	# @return True => connected, False => not connected
	def IsConnected(self):
		return self._isConnected


	##
	# Close the the database connection, if it's open.
	def CloseConnection(self):
		if self._isConnected == True:
			self._dbObject.close()
			self._isConnected = False


	##
	# Query the database, which will return a tuple in the form of:
	# (resultsSet, errorMessage, lastRowID}
	# resultsSet : This is an results set object if it's a SQL such as SELECT,
	#              otherwise it's empty.
	# errorMessage : Error message if a failure or error is detected, otherwise
	#                it's None.
	# lastRowID : Row ID if an INSERT statement and returnLastRowID is set.
	# @query SQL query string
	# @commit Optional argument to perform a commit only needed if data change.
	# @returnLastRowID Optional argument to retrieve Last row ID if INSERT.
	# @return a tuple as described.
	def Query(self, query, commit=False, returnLastRowID = False):
		cursor = self._dbObject.cursor()
		rowID = None

		try:
			cursor.execute(query)
			
		except mysql.connector.errors.ProgrammingError as err:
			cursor.close()
			return (None, err, rowID)

		# Get the column names
		columnNames = cursor.column_names
		
		# Create the results set to add rows to.
		resultsSet = ResultsSet()
		
		for row in cursor:
		
			resultRow = ResultsRow()
			
			colNo = 0
			for column in row:
				col = ResultColumn(columnNames[colNo], column)
				resultRow.AddColumn(col)
				colNo += 1

			resultsSet.AddRow(resultRow)

		# If commit is required, then perform it.
		if commit == True:
			# Make sure data is committed to the database
			self._dbObject.commit()

		# If the last row ID is needed, return it.
		if returnLastRowID == True:
			rowID = cursor.lastrowid
		
		# Close the cursor object.
		cursor.close()

		return (resultsSet, None, rowID)
