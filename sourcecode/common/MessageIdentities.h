//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef MESSAGEIDENTITIES_H
#define MESSAGEIDENTITIES_H
#include <MsgProtocolDefs.h>


namespace MessageProtocol
{

const MessageID BaseMessageID_State = 0xFA0;
const MessageID MsgID_StartHandshake = BaseMessageID_State + 0x0A;
const MessageID MsgID_HandshakeResponse = BaseMessageID_State + 0x14;
const MessageID MsgID_AuthenticateUser = BaseMessageID_State + 0x1E;
const MessageID MsgID_AuthenticationStatus = BaseMessageID_State + 0x28;
const MessageID MsgID_BeginProjectsBulkImport = BaseMessageID_State + 0x32;
const MessageID MsgID_ProjectsListBulkData = BaseMessageID_State + 0x3C;
const MessageID MsgID_SelectProject = BaseMessageID_State + 0x46;
const MessageID MsgID_ProjectSelectionStatus = BaseMessageID_State + 0x50;
const MessageID MsgID_BeginProjectAssetsBulkData = BaseMessageID_State + 0x5A;
const MessageID MsgID_ProjectAssetsBulkData = BaseMessageID_State + 0x64;
const MessageID MsgID_BeginReleasesBulkData = BaseMessageID_State + 0x6E;
const MessageID MsgID_ReleasesBulkData = BaseMessageID_State + 0x78;

}   //  namespace MessageProtocol

#endif  //  #ifndef MESSAGEIDENTITIES_H
