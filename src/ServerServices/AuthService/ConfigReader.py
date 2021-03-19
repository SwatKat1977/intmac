#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import sys
from xml.etree import ElementTree
from AuthenticatorConfig import *

tree = None

# Top-level XML tags
XMLTag_Database = "DATABASE"
XMLTag_Network = "NETWORK"
XMLTag_ServiceDiscovery = "SERVICEDISCOVERY"

XMLBooleanTagAttrib_True = "true"
XMLBooleanTagAttrib_False = "false"

# Database XML Tag attributes
XMLDatabaseTagAttrib_Username = "username"
XMLDatabaseTagAttrib_Password = "password"
XMLDatabaseTagAttrib_DBName = "name"

# Service Discovery XML Tag attributes
XMLServiceDiscoveryTagAttrib_Service = "service"
XMLServiceDiscoveryTagAttrib_Mode = "mode"
XMLServiceDiscoveryTagAttrib_port = "port"
XMLServiceDiscoveryTagAttrib_ConnectionType = "connectionType"

# Service Discovery XML Tag Service attribute values
XMLServiceDiscoveryTagServiceAttrib_BusinessLogic = "businessLogic"
XMLServiceDiscoveryTagServiceAttrib_Gateway = "gateway"

# Service Discovery XML Tag Connection Type attribute values
XMLServiceDiscoveryTagConnTypeAttrib_Raw = "raw"
XMLServiceDiscoveryTagConnTypeAttrib_SSL = "ssl"

		
# <summary>
# </summary>
class ConfigReader(object):

	# <summary>
	# </summary>
	def __init__(self):
		self.__configObject = AuthenticatorConfig()

		
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
		self.__configObject.DatabaseUsername = username
		self.__configObject.DatabasePassword = password
		self.__configObject.DatabaseName = dbName
	
		return None


	# <summary>
	# Process an XML element node that is for a service discovery entry.
	# </summary>
	# <param name="node">XML node</param>
	def ProcessServiceDiscoveryNode(self, node):
		config = ServiceConfig()
 
		service = node.attrib.get(XMLServiceDiscoveryTagAttrib_Service)
		mode = node.attrib.get(XMLServiceDiscoveryTagAttrib_Mode)
		port = node.attrib.get(XMLServiceDiscoveryTagAttrib_port)
		connType = node.attrib.get(XMLServiceDiscoveryTagAttrib_ConnectionType)

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
		if connType == XMLServiceDiscoveryTagConnTypeAttrib_Raw:
			config.NetworkConnectionType = NetworkConnectionType.Raw
		elif connType == XMLServiceDiscoveryTagConnTypeAttrib_SSL:
			config.NetworkConnectionType = NetworkConnectionType.SSL
		else:
			return "Network connection type '{0}' for {1} service is invalid" \
			.format(connType, service)

		# Check that the service name is valid.
		if service == XMLServiceDiscoveryTagServiceAttrib_BusinessLogic:
			self.__configObject.Service_BusinessLogic = config
		elif service == XMLServiceDiscoveryTagServiceAttrib_Gateway:
			self.__configObject.Service_Gateway = config
		else:
			return "Invalid service discovery service name '{0}'".format(service)

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

			# Node Type : Service Discovery
			elif nodeName == XMLTag_ServiceDiscovery:
				status = self.ProcessServiceDiscoveryNode(node)
				if status != None:
					return (None, status)

			else:
				errMsg = "Invalid config item : '{0}'".format(nodeName)
				return (None, errMsg)

		return (self.__configObject, '')


	# <summary>
	# Parse a string what should containing a boolean value.
	# </summary>
	# <param name="filename">String containing boolean value.</param>
	def __ParseBoolString(self, string):
		return string.upper() == "TRUE"
