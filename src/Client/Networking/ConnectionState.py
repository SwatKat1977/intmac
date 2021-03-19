#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////


# <summary>
# Connection state enumeration.
# </summary>
class ConnectionState(object):

	# Disconnected state.
	Disconnected = 0
	
	# Connection has been made and the handshake has began.
	Handshaking = 1

	# The handshake has been completed, but no user has been authenticated.
	NotAuthenticated = 2

	# User authentication in progress, awaiting response.
	Authenticating = 3

	# Authentication has completed, domain and projects received, awaiting the
	# user to select the domain and project.
	SelectingProject = 4

	# Project has been selected, the bulk data is currently in the process of
	# being downloaded.
	DownloadingBulkData = 5
