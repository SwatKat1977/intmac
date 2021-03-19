#  Copyright 2014-2017 Integrated Test Management Centre Foundation Team
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  //////////////////////////////////////////////////////////////////////////
from BaseMessageIDs import *

# Messages sent to and from the client and gateway.
class Client2GatewayMsgID(object):
	
	#  Message Definitions - Handshake
	RequestHandshake = MsgID_ClientAndGatewayBase + 1
	HandshakeResponse = MsgID_ClientAndGatewayBase + 2

	# Message Definitions - Authentication
	SendLogonCreds = MsgID_ClientAndGatewayBase + 3
	LogonResponse = MsgID_ClientAndGatewayBase + 4
	ProjectsList = MsgID_ClientAndGatewayBase + 5
	ProjectSelection = MsgID_ClientAndGatewayBase + 6

	# Bulk data loading messages
	ProjectAssetsBulkData = MsgID_ClientAndGatewayBase + 7
	ProjectAssetsBulkDataResponse = MsgID_ClientAndGatewayBase + 8
	ProjectReleasesBulkData = MsgID_ClientAndGatewayBase + 9
	ProjectReleasesBulkDataResponse = MsgID_ClientAndGatewayBase + 10

	# Message Definitions - Service
	KeepAlive = MsgID_ClientAndGatewayBase + 901


# Tags for 'RequestHandshake' message.
MsgTag_RequestHandshake_Protocol = 'Protocol'
MsgTag_RequestHandshake_Client = 'Client'

# Tags for 'HandshakeResponse' message.
MsgTag_HandshakeResponse_ResponseStatus = 'ResponseStatus'
MsgTag_HandshakeResponse_StatusStr = 'StatusStr'
HandshakeResponseMsgs = [MsgTag_HandshakeResponse_ResponseStatus]

# Tags for 'SendLogonCreds' message.
MsgTag_SendLogonCreds_Username = 'Username'
MsgTag_SendLogonCreds_Password = 'Password'

# Tags for 'LogonResponse' message.
MsgTag_LogonResponse_ResponseStatus = 'ResponseStatus'
MsgTag_LogonResponse_StatusStr = 'StatusStr'
LogonResponseMsgs = [MsgTag_LogonResponse_ResponseStatus, 	
	MsgTag_LogonResponse_StatusStr]

# Tags for 'ProjectsList' message.
MsgTag_ProjectsList_Projects = 'Projects'
ProjectsListMsgs = [MsgTag_ProjectsList_Projects]

# Tags for 'ProjectsList' message.
MsgTag_ProjectBulkDataBegin_TotalElements = 'TotalElements'
MsgTag_ProjectBulkDataBegin_TotalLength = 'TotalLength'
MsgTag_ProjectBulkDataBegin_NoOfMsgs = 'NoOfMsgs'

# Tags for 'ProjectsList' message.
MsgTag_ProjectSelection_SelectedProject = 'SelectedProject'
ProjectSelectionMsgs = [MsgTag_ProjectSelection_SelectedProject]

# Tags for 'ProjectAssetsBulkData' message.
MsgTag_ProjectAssetsBulkData_Assets = 'Assets'
ProjectAssetsBulkDataMsgs = [MsgTag_ProjectAssetsBulkData_Assets]
