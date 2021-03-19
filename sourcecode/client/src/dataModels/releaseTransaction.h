//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef RELEASETRANSACTION_H
#define RELEASETRANSACTION_H

#include <string>
#include <dataModels/iReleaseData.h>


enum ReleaseTransactionType
{
    ReleaseTransactionType_add,
    ReleaseTransactionType_delete,
    ReleaseTransactionType_dataModified
};


class ReleaseTransaction
{
public:
    ReleaseTransaction( ReleaseItemID ID
                      , ReleaseItemID parentID
                      , std::string title
                      , ReleaseTransactionType transType
                      , ReleaseItemType itemType );

    ~ReleaseTransaction();

    ReleaseItemID GetID() { return m_ID; }
    
    ReleaseItemID GetParentID() { return m_parentID; }

    std::string GetTitle() { return m_title; }

    ReleaseTransactionType GetTransactionType() { return m_type; }

    ReleaseItemType GetItemType() { return m_itemType; }

protected:
    std::string m_title;
    ReleaseItemID m_ID;
    ReleaseItemID m_parentID;
    ReleaseTransactionType m_type;
    ReleaseItemType m_itemType;
};

#endif
