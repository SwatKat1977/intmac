#  Copyright 2014-2018 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
import signal
import time
import threading
import errno
from twisted.internet import reactor
from twisted.internet.task import LoopingCall
from AppFramework.Application import *
from Singleton import *
from Version import ServiceVersion
from ConfigReader import *
from Logger.Logger import LoggerInst, LogLevel, AllLogLevels


TitleStr = "Integrated Test Management Centre Gateway Service V{0}"
CopyrightStr = "Copyright (c) 2014 - 2018 Integrated Test Management Centre \
 Foundation Team"
LicenseStr = "Licensed under Apache 2 license \
[http://www.apache.org/licenses/LICENSE-2.0]"


class SvcConnState(object):
	Disconnected = 0
	Connected = 1
	Disabled  = 2
	

@Singleton
class GatewayServiceApp(Application):

	MainProcessLoopInterval = 1

	## ConfigFile property (getter)
	@property
	def ConfigFile(self):
		return self.__configFile

	## ConfigFile property (setter).
	@ConfigFile.setter
	def ConfigFile(self, value):
		self.__configFile = value

	## CommandLineArguments property (getter)
	@property
	def CommandLineArguments(self):
		return self.__commandLineArguments


	## Gateway service (default) constructor.
	#  @param self The object pointer.
	def __init__(self):
		Application.__init__(self)

		self.__commandLineArguments = None
		self.__mainProcessLoop = None
		self.__configFile = 'config.xml'

		self.__clientInterfaceOKPrevState = False
		self.__authInterfaceOKPrevState  = False
		self.__clientInterfaceOK = False
		self.__authInterfaceOK = False


	# <summary>
	# </summary>
	def ConnectionToAuthServiceFailed(self, reason):
		self.LogEvent(LogLevel.Error,
		 "Unable to connect to Authentication Service, retrying...")
		reactor.callLater(20, self.ConnectToAuthService)


	# <summary>
	# </summary>
	def ConnectToAuthService(self):
		LoggerInst.Log(LogLevel.Info, 'calling ConnectToAuthService()')
		from twisted.internet.endpoints import TCP4ClientEndpoint
		endpoint = TCP4ClientEndpoint(reactor, 
			self._config.Service_Auth.Hostname,
			self._config.Service_Auth.NetworkPort)
		d = endpoint.connect(self.__authInterfaceFactory)
		d.addErrback(self.ConnectionToAuthServiceFailed)


	# <summary>
	# </summary>
	def StartClientInterfaceSuccessful(self, reason):
		self.__clientInterfaceOK = True
		LoggerInst.Log(LogLevel.Info, 'Client Interface successfully created')


	# <summary>
	# </summary>
	def StartClientInterfaceFailed(self, reason):
		LoggerInst.Log(LogLevel.Emergency,
			'Unable to create client interface, reason : {0}'.format(
			reason.getBriefTraceback))
		reactor.stop()


	# <summary>
	# </summary>
	def StartClientInterface(self):
		from twisted.internet.endpoints import TCP4ServerEndpoint
		from Network.ClientInterfaceMsgProcessor import ClientInterfaceMsgProcessor
		from Network.ClientInterface import ClientInterfaceFactory
		from Protocol.Client2Gateway import Client2GatewayMsgID
	
		# Message processor object.
		msgProcessor = ClientInterfaceMsgProcessor()

		# Register known message ID's.
		msgProcessor.RegisterMessageID(Client2GatewayMsgID.RequestHandshake)
		msgProcessor.RegisterMessageID(Client2GatewayMsgID.SendLogonCreds)
		msgProcessor.RegisterMessageID(Client2GatewayMsgID.ProjectSelection)
		msgProcessor.RegisterMessageID(Client2GatewayMsgID.KeepAlive)

		# Create the client interface using the network port from config.
		self.__clientInterface = TCP4ServerEndpoint(reactor,
			self._config.ClientInterface.NetworkPort)

		# Start listening on designated port.
		deferred = self.__clientInterface.listen(ClientInterfaceFactory(msgProcessor))
		deferred.addCallback(self.StartClientInterfaceSuccessful)
		deferred.addErrback(self.StartClientInterfaceFailed)


	## Gateway service initialisation functionality.
	#  @param self The object pointer.
	def _Initialise(self):
		from Logger.ConsoleLogPrint import ConsoleLogPrint
		consoleLogPrint = ConsoleLogPrint('ConsoleLogPrint', AllLogLevels)
		LoggerInst.RegisterSink(consoleLogPrint)

		LoggerInst.Log(LogLevel.Info, TitleStr, ServiceVersion)
		LoggerInst.Log(LogLevel.Info, CopyrightStr)
		LoggerInst.Log(LogLevel.Info, LicenseStr)

		# Create authentication network interface message processor and the
		# related message factory.
		from Network.AuthInterfaceMsgProcessor import AuthInterfaceMsgProcessor
		from Network.AuthInterface import AuthInterfaceFactory
		self.__authMsgProcessor = AuthInterfaceMsgProcessor()
		self.__authInterfaceFactory = AuthInterfaceFactory(
			self.__authMsgProcessor, self)

		# Initialise the authentication service network connection state.
		self.__svcConnState_Auth = SvcConnState.Disconnected

		###############################
		# Service level 0 : Bootstrapping
		###############################
		LoggerInst.Log(LogLevel.Info,
			"Entering service level 0 - Bootstrapping")

		# Set the signal handler.
		signal.signal(signal.SIGINT, self.SignalHandler)

		LoggerInst.Log(LogLevel.Info,
		"Reading configuration file '{0}'".format(self.ConfigFile))

		# Create an instance of the config reader and then parse the XML file.
		configReader = ConfigReader()
		self._config, errCode = configReader.ReadXMLConfig(self.ConfigFile)

		if self._config == None:
			failStr = "Config file read failed : {0}".format(errCode)
			LoggerInst.Log(LogLevel.Emergency, failStr)
			return False

		###############################
		# Service level 1 : Bringing up all network connections
		###############################
		LoggerInst.Log(LogLevel.Info,
			"Entering service level 1 - Bringing up all network connections")

		# Check if the service 'authenticator' is set to disabled in the
		# configuration file, then set it as disabled so it won't connect.
		if self._config.Service_Auth.DiscoveryMode == DiscoveryMode.Disabled:
			self.__svcConnState_Auth = SvcConnState.Disabled
			LoggerInst.Log(LogLevel.Info,
				"Service 'Authenticator' Interface [Disabled]")
			self.__authInterfaceOK = True

		else:
			LoggerInst.Log(LogLevel.Info,
				"Service 'Authenticator' Interface [Auto]")
			# Attempt to connect to the authentication service when running.
			reactor.callLater(0.5, self.ConnectToAuthService)

		# If the client interface is enabled then start listening.
		if self._config.ClientInterface.StartState == True:
			LoggerInst.Log(LogLevel.Info, "Client Interface : Enabled")
			reactor.callLater(0.5, self.StartClientInterface) 

		else:
			LoggerInst.Log(LogLevel.Info, "Client Interface : Disabled")
			self.__clientInterfaceOK = True

		# Set to running.
		self.IsRunning = True

		return True


	## Main service loop.
	#  @param self The object pointer.
	def _MainLoop(self):
		self.__mainProcessLoop = LoopingCall(self.__MainProcessLoop)
		self.__mainProcessLoop.start(self.MainProcessLoopInterval)

		reactor.run(installSignalHandlers = False)
		self.IsRunning = False


	## Main Process loop.
	#  @param self The object pointer.
	def __MainProcessLoop(self):

		# If the authentication service or client interace needs to be started
		# and isn't, so wait!
		if self.__authInterfaceOK == False or self.__clientInterfaceOK == False:
			return
			
		if (self.__authInterfaceOK == True and \
			self.__authInterfaceOKPrevState == False) or \
			(self.__clientInterfaceOK == True and \
			self.__clientInterfaceOKPrevState == False):
			
			# Reset previous state values.
			self.__clientInterfaceOKPrevState = True
			self.__authInterfaceOKPrevState  = True
			
			# Announce we have left Service level 1
			LoggerInst.Log(LogLevel.Info,
			"Entering service level 2 - Open")

		try:
			pass

		except IOError, e:
			if e.errno == errno.EINTR:
				pass


	# <summary>
	# Service shutdown functionality.
	# </summary>
	def _Shutdown(self):
		LoggerInst.Log(LogLevel.Info, "Shutting down service...")
		

	# <summary>
	# Service signal handling functionality.
	# </summary>
	# <param name="frame">Unused</param>
	def SignalHandler(self, signum, frame):
		self.IsRunning = False
		reactor.stop()


Gateway = GatewayServiceApp.Instance()
