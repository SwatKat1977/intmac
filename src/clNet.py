#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
from twisted.internet import reactor
import signal, os
import sys
from Client.Networking.GatewayInterfaceFactory import GatewayInterfaceFactory

from Common.Protocol.Client2Gateway import Client2GatewayMsgID, \
	HandshakeResponseMsgs, LogonResponseMsgs, ProjectsListMsgs, \
	ProjectAssetsBulkDataMsgs
from Client.Networking.GatewayInterfaceMsgProcessor import GatewayInterfaceMsgProcessor
____msgProcessor = GatewayInterfaceMsgProcessor()

# Register known message ID's.
____msgProcessor.RegisterMessage(Client2GatewayMsgID.HandshakeResponse,
		HandshakeResponseMsgs)
____msgProcessor.RegisterMessage(Client2GatewayMsgID.LogonResponse,
		LogonResponseMsgs)
____msgProcessor.RegisterMessage(Client2GatewayMsgID.ProjectsList,
		ProjectsListMsgs)
____msgProcessor.RegisterMessage(Client2GatewayMsgID.ProjectAssetsBulkData,
		ProjectAssetsBulkDataMsgs)


gatewayInterfaceFactory = GatewayInterfaceFactory(____msgProcessor)

# <summary>
# </summary>
def ConnectionToAuthServiceFailed(reason):
	print("[Error] Unable to connect to gateway retrying...")
	reactor.callLater(20, reactor.callFromThread, ConnectToAuthService)


# <summary>
# </summary>
def ConnectToAuthService():
	from twisted.internet.endpoints import TCP4ClientEndpoint
	endpoint = TCP4ClientEndpoint(reactor, 'localhost', 7000)
	d = endpoint.connect(gatewayInterfaceFactory)
	d.addErrback(ConnectionToAuthServiceFailed)


def SignalHandler(signum, frame):
	print("[INFO] Shutting down service...")
	reactor.callFromThread(reactor.stop)
	networkThread.join()
	sys.exit()

# Set the signal handler.
signal.signal(signal.SIGINT, SignalHandler)

		
import threading
# Starting reactor in a thread...
print("[INFO] Bringing up all network connections")
networkThread = threading.Thread(target=reactor.run, 
	kwargs={"installSignalHandlers": False})
networkThread.start()

reactor.callFromThread(ConnectToAuthService) 

print("[info entering loop")

import time
while True:
	try:
		time.sleep(0.01)
	except IOError, e:
		if e.errno == errno.EINTR:
			pass
