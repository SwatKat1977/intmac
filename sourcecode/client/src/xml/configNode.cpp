//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <xml/ConfigNode.h>


ConfigNode::ConfigNode( iDOMElement *node )
    : m_node( node )
    , m_attributeIndex( 0 )
{
    m_attributes = m_node->getAttributes( );
}


ConfigNode::~ConfigNode()
{
}


std::string ConfigNode::GetKeyName()
{
    return m_node->getNodeName( );
}


std::string ConfigNode::GetKeyValue()
{
    std::string keyValue;
    iDOMNode *element = NULL;
    
    element = m_node->getFirstChild();

    if ( element )
    {
        keyValue = element->getNodeValue();
    }

    return keyValue;
}


bool ConfigNode::HasAttributes()
{
    return m_node->hasAttributes();
}


ConfigAttribute *ConfigNode::GetAttribute()
{
    ConfigAttribute *attr = NULL;

    if ( HasAttributes() == true )
    {
        iDOMAttr *attributeNode = m_attributes->item( m_attributeIndex );
        attr = new ConfigAttribute;

        attr->m_key =  attributeNode->getName();
        attr->m_value = attributeNode->getValue();
    }

    return attr;
}


size_t ConfigNode::GetTotalNumberOfAttributes()
{
    return m_attributes->getLength();
}


void ConfigNode::NextAttribute()
{
    if ( HasAttributes() == true )
    {
        if ( ( m_attributeIndex + 1 ) <= m_attributes->getLength() )
        {
            m_attributeIndex++;
        }
    }
}
