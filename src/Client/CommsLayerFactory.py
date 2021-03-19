#  Copyright 2014-2016 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
from Common.RawSocket.IPV4SocketLayer import *


class CommsLayerType(object):
    RawIPV4Network = 0


# ***********************************************************************
# Definition of Comms Layer Factory class
# ***********************************************************************
class CommsLayerFactory:

	##
	# Class constructor.
	def __init__(self):
		self.rawIPV4SocketLayer = IPV4SocketLayer()
		

	##
	# Create a comms layer for a specific comms type.
	# @param commsType Message receiver class.
	# @param comms layer instance or None if type isn't know.
	def Create(self, commsType):

		if commsType == CommsLayerType.RawIPV4Network:
			return self.rawIPV4SocketLayer.CreateSocket()

		# default if comms type is unknown.
		return None
