//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <hci/mainDialog/assets/toolbarItemData.h>


ToolbarItemData::ToolbarItemData( int id
                                , ToolbarItemSubType subType
                                , std::string name )
                                : m_itemID( id )
                                , m_subType( subType )
                                , m_name( name )
{
}


ToolbarItemData::~ToolbarItemData()
{
}
