//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef CONFIGNODE_H
#define CONFIGNODE_H

#include <iDOMElement.h>


// *************************************************************************
// * ConfigAttribute Class Definition
// *************************************************************************
class ConfigAttribute
{
public:

    ConfigAttribute( std::string key, std::string value ) 
        : m_key( key )
        , m_value( value )
    { }

    ConfigAttribute( ) { }

    std::string m_key;
    std::string m_value;
};


class ConfigNode
{
public:

   ConfigNode( iDOMElement *node );

    ~ConfigNode();

    std::string GetKeyName( );

    std::string GetKeyValue( );

    bool HasAttributes( );

    ConfigAttribute *GetAttribute( );

    size_t GetTotalNumberOfAttributes( );
 
    size_t GetCurrentAttributeIndex( ) { return m_attributeIndex; }
    
    void NextAttribute( );

protected:
    iDOMElement *m_node;
    iDOMNamedNodeMap *m_attributes;
    size_t m_attributeIndex;
};

#endif
