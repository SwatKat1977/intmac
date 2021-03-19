#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////

class DiscoveryMode(object):
	Auto = 0
	Disabled = 1


class NetworkConnectionType(object):
	Raw = 0
	SSL = 1


# <summary>
# </summary>
class ServiceConfig(object):

	# <summary>
	# Get discovery mode.
	# </summary>
	@property
	def DiscoveryMode(self):
		return self._DiscoveryMode


	# <summary>
	# Set discovery mode.
	# </summary>
	@DiscoveryMode.setter
	def DiscoveryMode(self, value):
		self._DiscoveryMode = value


	# <summary>
	# Get Network port.
	# </summary>
	@property
	def NetworkPort(self):
		return self._networkPort


	# <summary>
	# Set Network port.
	# </summary>
	@NetworkPort.setter
	def NetworkPort(self, value):
		self._networkPort = value


	# <summary>
	# Get Network connection type.
	# </summary>
	@property
	def NetworkConnectionType(self):
		return self._networkConnectionType


	# <summary>
	# Set Network connection type.
	# </summary>
	@NetworkConnectionType.setter
	def NetworkConnectionType(self, value):
		self._networkConnectionType = value


	def __init__(self):
		self._DiscoveryMode = DiscoveryMode.Auto
		self._networkPort = 0
		self._networkConnectionType = NetworkConnectionType.Raw


class AuthenticatorConfig(object):
	@property
	def DatabaseUsername(self):
		return self._dbUsername


	@DatabaseUsername.setter
	def DatabaseUsername(self, value):
		self._dbUsername = value


	@property
	def DatabasePassword(self):
		return self._dbPassword


	@DatabasePassword.setter
	def DatabasePassword(self, value):
		self._dbPassword = value

		
	@property
	def DatabaseName(self):
		return self._dbName


	@DatabaseName.setter
	def DatabaseName(self, value):
		self._dbName = value


	@property
	def Service_BusinessLogic(self):
		return self._service_BusinessLogic


	@Service_BusinessLogic.setter
	def Service_BusinessLogic(self, value):
		self._service_BusinessLogic = value


	@property
	def Service_Gateway(self):
		return self._service_Gateway


	@Service_Gateway.setter
	def Service_Gateway(self, value):
		self._service_Gateway = value


	def __init__(self):
		self._dbUsername = None
		self._dbPassword = None
		self._dbName = None
		self._service_BusinessLogic = DiscoveryMode.Auto
		self._service_Gateway = DiscoveryMode.Auto
