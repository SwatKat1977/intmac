//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef CLIENTSERVERMESSAGES_STATE
#define CLIENTSERVERMESSAGES_STATE

const int MaxBulkDataTxPayload = 500;

struct CSMsg_AuthenticationStatus
{
    int m_Status;
};


struct CSMsg_BeginProjectsBulkImport
{
    int m_totalPayloadSize;
};


struct CSMsg_ProjectSelectionStatus
{
    int m_Status;
};


struct CSMsg_BeginProjectAssetsBulkData
{
    int m_totalPayloadSize;
};


struct CSMsg_BeginReleasesBulkData
{
    int m_totalPayloadSize;
};

#endif
