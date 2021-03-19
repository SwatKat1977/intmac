#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
from twisted.internet import protocol
from GatewayInterfaceProtocol import *


# <summary>
# Twisted interface factory for connections between a client and the gateway.
# </summary>
class GatewayInterfaceFactory(protocol.Factory):

	# <summary>
	# Gateway Interface Factory 
	# </summary>
	def __init__(self, msgProcessor, systemObject):
		self.msgProcessor = msgProcessor
		self.connection = None
		self.systemObject = systemObject


	# <summary>
	# Build a protocol and return it.
	# </summary>
	# <param name="addr">Unused</param>
	def buildProtocol(self, addr):
		return GatewayInterfaceProtocol(self, self.msgProcessor,
			self.systemObject)
