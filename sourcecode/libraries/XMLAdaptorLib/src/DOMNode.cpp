//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <DOMNode.h>


DOMNode::DOMNode( xercesc::DOMNode *node ) : m_nodeWrapper( node )
{
}


DOMNode::~DOMNode( )
{
}


std::string DOMNode::getNodeName( )
{
    char *name = xercesc::XMLString::transcode( m_nodeWrapper->getNodeName( ) );
    std::string nodeName( name );
    xercesc::XMLString::release( &name );

    return nodeName;
}


DOMNode *DOMNode::getFirstChild()
{
    return new DOMNode( m_nodeWrapper->getFirstChild( ) ); 
}


std::string DOMNode::getNodeValue()
{
    char *nodeValue = xercesc::XMLString::transcode( m_nodeWrapper->getNodeValue( ) );
    std::string nodeNameStr( nodeValue );
    xercesc::XMLString::release( &nodeValue );

    return nodeNameStr;
}
