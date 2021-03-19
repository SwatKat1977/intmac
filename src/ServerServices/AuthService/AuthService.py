#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import signal, os
import errno
import time
import threading
from twisted.internet.endpoints import TCP4ServerEndpoint
from Common.Service.ServiceBase import *
from Version import *
from Networking.GatewayInterface import *
from Common.Protocol.Gateway2AuthIDs import *
from ConfigReader import *
from Common.Singleton import *
from ServiceEvents import *


TitleStr = "Integrated Test Management Centre Authenticator Service V{0}"
CopyrightStr = "Copyright (c) 2014 - 2017 Integrated Test Management Centre \
 Foundation Team"
LicenseStr = "Licensed under Apache 2 license \
[http://www.apache.org/licenses/LICENSE-2.0]"

class SvcConnState(object):
	Disconnected = 0
	Connected = 1
	Disabled  = 2

# <summary>
# Authentication service class implementation, which is a singleton.
# </summary>
@Singleton
class AuthService(ServiceBase):


	# <summary>
	# Get config file.
	# </summary>
	@property
	def ConfigFile(self):
		return self._configFile


	# <summary>
	# Set config file.
	# </summary>
	@ConfigFile.setter
	def ConfigFile(self, value):
		self._configFile = value


	# <summary>
	# Authentication service (default) constructor.
	# </summary>
	def __init__(self):
		ServiceBase.__init__(self)
		
		# Configuration items.
		self._config = None
		
		# Shutdown has been requested flag
		self.__shutdownRequested = False
		
		self.__svcConnState_BusinessLogic = SvcConnState.Disconnected
		self.__svcConnState_Gateway = SvcConnState.Disconnected
		
		# Thread to run the Twisted reactor in.
		self._networkThread = None
		
		# Instantiate MessageProcessor class for gateway.
		from Networking.GatewayInterfaceMsgProcessor \
			import GatewayInterfaceMsgProcessor
		self.__gatewayMsgProcessor = GatewayInterfaceMsgProcessor()

		# Endpoints for the service connections.
		self._gatewayConn = None
		self._businessLogicConn = None

		# Configuration file name & path.
		self._configFile = 'config.xml'


	# <summary>
	# Handle service mode changes, by default it will just set the new mode
	# that comes from the event passed in.  Additional functionality could be
	# added in a derived class.
	# </summary>
	# <param name="event">Service event object</param>
	def HandleModeChange(self, event):
		#self.LogEvent(LogLevel.Informational, "Mode changed from '{0}' to '{1}'",
		#	ServiceModeStr[self.ServiceMode], ServiceModeStr[event.Data])
		self.ServiceMode = event.Data


	# <summary>
	# Initialise the authentication service, which includes reading the XML
	# configuration file, starting the required network interfaces and then
	# setting service to open.
	# </summary>
	def Initialise(self):
		self.LogEvent(LogLevel.Informational, TitleStr, ServiceVersion)
		self.LogEvent(LogLevel.Informational, CopyrightStr)
		self.LogEvent(LogLevel.Informational, LicenseStr)	

		self.LogEvent(LogLevel.Informational, 
		"Entering service level 0 - Bootstrapping")

		# Set the signal handler.
		signal.signal(signal.SIGINT, self.SignalHandler)

		self.LogEvent(LogLevel.Informational,
		"Reading configuration file 'config.xml'")

		# Create an instance of the config reader and then parse the XML file.
		configReader = ConfigReader()
		self._config, errCode = configReader.ReadXMLConfig(self.ConfigFile)

		if self._config == None:
			failStr = "Config file read failed : {0}".format(errCode)
			self.LogEvent(LogLevel.Emergency, failStr)
			return False

		# Register 'Start handshake' event.
		self.__gatewayMsgProcessor.RegisterMessageID(MsgID_StartHandshake)
		
		# Register 'End handshake' event.
		self.__gatewayMsgProcessor.RegisterMessageID(MsgID_EndHandshake)

		# Opening connection to the database
		self.LogEvent(LogLevel.Informational,
		"Database Connection : Not implemented")
		
		# Change mode to 'Opening Network Connections'
		self.ReceiveEvent(ServiceEvent(0, ServiceEventType.ModeChange,
			ServiceMode.OpeningNetworkConnections))
		self.LogEvent(LogLevel.Informational,
		"Service Mode changed to 'Opening Network Connections'")
		
		# Start state of all network connection states.
		networkConnsOpen = False
		
		# Check if the service 'Business Logic' is disabled in the config file
		# then set it as disabled so it won't connect.
		if self._config.Service_BusinessLogic.DiscoveryMode == DiscoveryMode.Disabled:
			self.__svcConnState_BusinessLogic = SvcConnState.Disabled
			self.LogEvent(LogLevel.Informational,
			"Service 'Business Logic' Interface [Disabled]")
		else:
			self.LogEvent(LogLevel.Informational,
			"Service 'Business Logic' Interface [Auto]")

		# Check if the service 'Gateway' is disabled in the config file then
		# set it as disabled so it won't connect.
		if self._config.Service_Gateway.DiscoveryMode == DiscoveryMode.Disabled:
			self.__svcConnState_Gateway = SvcConnState.Disabled
			self.LogEvent(LogLevel.Informational,
			"Service 'Gateway' Interface : Disabled")
			networkConnsOpen = True
		else:
			self.LogEvent(LogLevel.Informational,
			"Service 'Gateway' Interface [Auto]")
			
			gatewayConf = self._config.Service_Gateway
			self._gatewayConn = TCP4ServerEndpoint(reactor,
				gatewayConf.NetworkPort)
			self._gatewayConn.listen(GatewayInterfaceFactory(
				self.__gatewayMsgProcessor))

		self.LogEvent(LogLevel.Informational,
		 "Bringing up all network connections")

		self._networkThread = threading.Thread(target=reactor.run,
			kwargs={"installSignalHandlers": False})
		self._networkThread.start()

		# Try to open any required network connections.
		while networkConnsOpen == False:

			try:
				time.sleep(0.01)
			except KeyboardInterrupt:
				if e.errno == errno.EINTR:
					pass

			# If all of the services are either connected or disabled then
			# continue bringing up the service.
			if (self.__svcConnState_Gateway == SvcConnState.Disabled or \
				self.__svcConnState_Gateway == SvcConnState.Connected) and \
				(self.__svcConnState_BusinessLogic == SvcConnState.Disabled or \
				self.__svcConnState_BusinessLogic == SvcConnState.Connected):
				networkConnsOpen = True
		
			if self.__shutdownRequested == True:
				return False

		if self.__svcConnState_Gateway == SvcConnState.Disabled:
			self.LogEvent(LogLevel.Informational,
			 "Gateway disabled : No network connections open")
		else:
			self.LogEvent(LogLevel.Informational,
			 "All network connections open")

		self.ReceiveEvent(ServiceEvent(0, ServiceEventType.ModeChange,
		ServiceMode.StartingService))
		self.LogEvent(LogLevel.Informational,
			"Service Mode changed to 'StartingService'")

		self.ReceiveEvent(ServiceEvent(0, ServiceEventType.ModeChange,
		ServiceMode.Open))
		self.LogEvent(LogLevel.Informational, "Service Mode changed to 'Open'")

		# Set the service as running.
		self.ServiceRunning = True

		return True


	# <summary>
	# The main execution loop of the authentication service.
	# </summary>
	def MainLoop(self):
		try:
			time.sleep(0.01)
		except IOError, e:
			if e.errno == errno.EINTR:
				pass


	# <summary>
	# Shutdown of authentication service.
	# </summary>
	def Shutdown(self):
		self.LogEvent(LogLevel.Informational, "Shutting down service...")

		if self._networkThread != None:
			# Shutdown the Twisted reactor and it's thread.
			reactor.callFromThread(reactor.stop)
			self._networkThread.join()


	# <summary>
	# Handle signal requests (e.g. HUP, INT)
	# </summary>
	# <param name="signum">Signal number</param>
	# <param name="frame">Unused</param>
	def SignalHandler(self, signum, frame):
		self.ServiceRunning = False
		self.__shutdownRequested = True


	# <summary>
	# Handle custom service events.
	# </summary>
	# <param name="event">Service event object</param>
	def HandleCustomEvent(self, event):
		if event.ID == EventID_GatewayConnected:
			self.LogEvent(LogLevel.Informational,
			"Gateway Interface : Connected..")
			self.__svcConnState_Gateway = SvcConnState.Connected
		
		elif event.ID == EventID_GatewayDisconnected:
			self.LogEvent(LogLevel.Informational,
			"Gateway Interface : Disconnected..")
			self.__svcConnState_Gateway = SvcConnState.Disconnected


AuthenticationService = AuthService.Instance()