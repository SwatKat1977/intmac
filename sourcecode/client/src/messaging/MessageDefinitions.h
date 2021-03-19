//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef MESSAGEDEFINITIONS_H
#define MESSAGEDEFINITIONS_H
#include <messaging\MessageDefinition.h>
#include <MessageIdentities.h>


namespace MessageProtocol
{

MessageProtocol::MessageDefinition MsgDef_HandshakeResponse( MsgID_HandshakeResponse
    , { MsgFilter_Any }
    , ClientServices::ClientService_sessionsManager );

MessageProtocol::MessageDefinition MsgDef_AuthenticationStatus( MsgID_AuthenticationStatus
    , { MsgID_HandshakeResponse }
    , ClientServices::ClientService_sessionsManager );

MessageProtocol::MessageDefinition MsgDef_BeginProjectsBulkImport( MsgID_BeginProjectsBulkImport
    , { MsgID_AuthenticationStatus }
    , ClientServices::ClientService_sessionsManager );

MessageProtocol::MessageDefinition MsgDef_ProjectsListBulkData( MsgID_ProjectsListBulkData
    , { MsgID_ProjectsListBulkData, MsgID_BeginProjectsBulkImport }
    , ClientServices::ClientService_sessionsManager);

MessageProtocol::MessageDefinition MsgDef_ProjectSelectionStatus( MsgID_ProjectSelectionStatus
    , { MsgID_ProjectsListBulkData }
    , ClientServices::ClientService_sessionsManager);

MessageProtocol::MessageDefinition MsgDef_BeginProjectAssetsBulkData( MsgID_BeginProjectAssetsBulkData
    , { MsgID_ProjectSelectionStatus }
    , ClientServices::ClientService_sessionsManager);

MessageProtocol::MessageDefinition MsgDef_ProjectAssetsBulkData( MsgID_ProjectAssetsBulkData
    , { MsgID_ProjectAssetsBulkData, MsgID_BeginProjectAssetsBulkData }
    , ClientServices::ClientService_sessionsManager);

}   //  namespace MessageProtocol

#endif  //  #ifndef MESSAGEDEFINITIONS_H

/*
switch ( header->m_ID )
{
case MessageProtocol::MsgID_HandshakeResponse:
returnStatus = HandleHandshakeResponse (header, body);
break;

case MessageProtocol::MsgID_AuthenticationStatus:
returnStatus = HandleAuthenticationStatus (header, body);
break;

case MessageProtocol::MsgID_ProjectSelectionStatus:
returnStatus = HandleProjectSelectionStatus (header, body);
break;

case MessageProtocol::MsgID_BeginProjectAssetsBulkData:
returnStatus = HandleBeginProjectAssets (header, body);
break;

case MessageProtocol::MsgID_ProjectAssetsBulkData:
returnStatus = HandleProjectAssetsBulkData (header, body);
break;

case MessageProtocol::MsgID_BeginReleasesBulkData:
returnStatus = HandleBeginReleasesBulkData (header, body);
break;

case MessageProtocol::MsgID_ReleasesBulkData:
returnStatus = HandleReleasesBulkData (header, body);
break;
*/

/*
Common::MessageTriggerMap MessageTriggers =
{
{ MsgID_StartHandshake, {} }
, { MsgID_HandshakeResponse, {} }
, { MsgID_AuthenticateUser, {} }
, { MsgID_AuthenticationStatus, {} }
, { MsgID_BeginProjectsBulkImport, { MsgTrigger_userAuthorised } }
, { MsgID_ProjectsListBulkData, { MsgTrigger_userAuthorised
, MsgTrigger_projectListBulkStarted } }
, { MsgID_SelectProject, { MsgTrigger_projectListLoaded } }
, { MsgID_ProjectSelectionStatus, { MsgTrigger_projectSelectionRequested } }
, { MsgID_BeginProjectAssetsBulkData, { MsgTrigger_projectSelected }}
, { MsgID_ProjectAssetsBulkData, { MsgTrigger_projectAssetsBulkDataStarted }}
, { MsgID_BeginReleasesBulkData, { MsgTrigger_projectAssetsLoaded }}
, { MsgID_ReleasesBulkData, { MsgTrigger_releasesBulkDataStarted }}
};

*/