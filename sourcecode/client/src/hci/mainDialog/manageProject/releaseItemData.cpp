//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <hci/mainDialog/manageProject/releaseItemData.h>


ReleaseItemData::ReleaseItemData( int id
                                , std::string name
                                , ReleaseItemType type )
                                : m_itemID( id )
                                , m_name( name )
                                , m_type( type )
{
}


ReleaseItemData::~ReleaseItemData()
{
}
