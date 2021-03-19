#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import sys
from xml.etree import ElementTree
from Configuration import *

tree = None

# Top-level XML tags
XMLTag_Database = "DATABASE"
XMLTag_ClientInterface = "CLIENTINTERFACE"
XMLTag_Service = "SERVICE"

XMLBooleanTagAttrib_True = "true"
XMLBooleanTagAttrib_False = "false"

XMLBooleanTagAttrib_Enabled = "enabled"
XMLBooleanTagAttrib_Disabled = "disabled"

# Client Interface XML Tag attributes
XMLClientInterfaceTagAttrib_StartState = "startState"
XMLClientInterfaceTagAttrib_Port = "port"
XMLClientInterfaceTagAttrib_ConnectionType = "connectionType"

# Database XML Tag attributes
XMLDatabaseTagAttrib_Username = "username"
XMLDatabaseTagAttrib_Password = "password"
XMLDatabaseTagAttrib_DBName = "name"

# Service Discovery XML Tag attributes
XMLServiceTagAttrib_Name = "name"
XMLServiceTagAttrib_Mode = "mode"
XMLServiceTagAttrib_Port = "port"
XMLServiceTagAttrib_ConnectionType = "connectionType"

# Service XML Tag Name attribute values
XMLServiceTagNameAttrib_BusinessLogic = "businessLogic"
XMLServiceTagNameAttrib_Auth = "auth"

# Service XML Tag Connection Type attribute values
XMLServiceTagConnTypeAttrib_Raw = "raw"
XMLServiceTagConnTypeAttrib_SSL = "ssl"

		
# <summary>
# </summary>
class ConfigReader(object):

	# <summary>
	# </summary>
	def __init__(self):
		self._configObject = Configuration()

		
	# <summary>
	# Process an XML element node that is for the database settings.
	# </summary>
	# <param name="node">XML node</param>
	def ProcessDatabaseNode(self, node):
		username = node.attrib.get(XMLDatabaseTagAttrib_Username)
		dbName = node.attrib.get(XMLDatabaseTagAttrib_DBName)
		password = node.attrib.get(XMLDatabaseTagAttrib_Password)

		# Check that the attributes exists, return error (None) if any are missing.
		if username == None or dbName == None or password == None:
			return "One or missing database configuration attributes is missing"

		# Update configuration object with database settings.
		self._configObject.DatabaseUsername = username
		self._configObject.DatabasePassword = password
		self._configObject.DatabaseName = dbName
	
		return None


	# <summary>
	# Process an XML element node that is for a service entry.
	# </summary>
	# <param name="node">XML node</param>
	def ProcessServiceNode(self, node):
		config = ServiceConfig()
 
		service = node.attrib.get(XMLServiceTagAttrib_Name)
		mode = node.attrib.get(XMLServiceTagAttrib_Mode)
		port = node.attrib.get(XMLServiceTagAttrib_Port)
		connType = node.attrib.get(XMLServiceTagAttrib_ConnectionType)

		# Check that the 'service' attribute. exists
		if service == None:
			return "'service' attribute missing from servicediscovery element"

		# Check that the 'mode' attribute. exists
		if mode == None:
			return "'mode' attribute missing from servicediscovery element"

		# Check that the 'port' attribute. exists
		if port == None:
			return "'port' attribute missing from servicediscovery element"

		# Check that the 'connectionType' attribute. exists
		if connType == None:
			return "'connectionType' attribute missing from servicediscovery element"

		# Convert the network port to an integer, catching invalid numbers.
		try:
			config.NetworkPort = int(port)
		except ValueError:
			return "Network port for {0} service isn't an integer".format(service)

		# Check that the mode is a valid string of 'auto' or 'disabled'.  If the
		# discovery mode is disabled then it won't try and establish a connection.
		if mode == 'auto':
			config.DiscoveryMode = DiscoveryMode.Auto
		elif mode == 'disabled':
			config.DiscoveryMode = DiscoveryMode.Disabled
		else:
			return "Invalid service discovery mode '{0}'".format(mode)

		# Check connection state is valid and convert it to an enumeration.
		if connType == XMLServiceTagConnTypeAttrib_Raw:
			config.NetworkConnectionType = NetworkConnectionType.Raw
		elif connType == XMLServiceTagConnTypeAttrib_SSL:
			config.NetworkConnectionType = NetworkConnectionType.SSL
		else:
			return "Network connection type '{0}' for {1} service is invalid" \
			.format(connType, service)

		# Check that the service name is valid.
		if service == XMLServiceTagNameAttrib_BusinessLogic:
			self._configObject.Service_BusinessLogic = config
		elif service == XMLServiceTagNameAttrib_Auth:
			self._configObject.Service_Auth = config
		else:
			return "Invalid service discovery service name '{0}'".format(service)

		return None


	# <summary>
	# 
	# </summary>
	# <param name="node">XML node</param>
	def ProcessClientInterfaceNode(self, node):
		config = ClientInterfaceConfig()
 
		startState = node.attrib.get(XMLClientInterfaceTagAttrib_StartState)
		port = node.attrib.get(XMLClientInterfaceTagAttrib_Port)
		connType = node.attrib.get(XMLClientInterfaceTagAttrib_ConnectionType)

		# Check that the 'startState' attribute. exists
		if startState == None:
			return "'startState' attribute missing from servicediscovery element"

		# Check that the 'port' attribute. exists
		if port == None:
			return "'port' attribute missing from servicediscovery element"

		# Check that the 'connectionType' attribute. exists
		if connType == None:
			return "'connectionType' attribute missing from servicediscovery element"

		# Convert the network port to an integer, catching invalid numbers.
		try:
			config.NetworkPort = int(port)
		except ValueError:
			return "Network port for {0} service isn't an integer".format(service)
		
		# Check start state is a valid ('enabled' or 'disabled').
		if startState == XMLBooleanTagAttrib_Enabled:
			config.StartState = True
		elif startState == XMLBooleanTagAttrib_Disabled:
			config.StartState = False
		else:
			return "Invalid client interface start state : '{0}'".format(startState)

		# Check connection state is valid and convert it to an enumeration.
		if connType == XMLServiceTagConnTypeAttrib_Raw:
			config.NetworkConnectionType = NetworkConnectionType.Raw
		elif connType == XMLServiceTagConnTypeAttrib_SSL:
			config.NetworkConnectionType = NetworkConnectionType.SSL
		else:
			return "Client Network connection type '{0}' for {1} service \
			is invalid".format(connType, service)

		self._configObject.ClientInterface = config

		return None


	# <summary>
	# Read the given XML file and parse it.
	# </summary>
	# <param name="filename">XML file to parse.</param>
	def ReadXMLConfig(self, filename):	
		try:
			with open(filename, 'rt') as f:
				tree = ElementTree.parse(f)
		
		# If there is an IO error (for example file not found) then return a
		# tuple of (None, raised exception).
		except IOError, ioExcept:
			return (None, ioExcept)

		# The XML file can't be parsed as it's not legal XML.  Return a tuple
		# of (None, raised exception).
		except ElementTree.ParseError, parseExcept:
			return (None, parseExcept)

		for node in tree.getroot():
			nodeName = node.tag.upper()

			# Node Type : Database
			if nodeName == XMLTag_Database:
				status = self.ProcessDatabaseNode(node)
				if status != None:
					return (None, status)

			# Node Type : Service
			elif nodeName == XMLTag_Service:
				status = self.ProcessServiceNode(node)
				if status != None:
					return (None, status)

			# Node Type : Client Interface
			elif nodeName == XMLTag_ClientInterface:
				status = self.ProcessClientInterfaceNode(node)
				if status != None:
					return (None, status)
	
			else:
				errMsg = "Invalid config item : '{0}'".format(nodeName)
				return (None, errMsg)

		if self._configObject.Service_Auth == None:
			return (None, 'Auth Service configuration is missing')

		if self._configObject.Service_BusinessLogic == None:
			return (None, 'Business Logic Service configuration is missing')

		return (self._configObject, '')


	# <summary>
	# Parse a string what should containing a boolean value.
	# </summary>
	# <param name="filename">String containing boolean value.</param>
	def __ParseBoolString(self, string):
		return string.upper() == "TRUE"
