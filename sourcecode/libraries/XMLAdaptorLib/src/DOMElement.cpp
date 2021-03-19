//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <DOMElement.h>


DOMElement::DOMElement( xercesc::DOMElement *element ) 
          : m_xercescDOMElement( element )
{
}


DOMElement::~DOMElement( )
{
}


iDOMElement *DOMElement::getFirstElementChild()
{
    return ( m_xercescDOMElement->getFirstElementChild() )
           ? new DOMElement( m_xercescDOMElement->getFirstElementChild() )
           : NULL;
}


iDOMElement *DOMElement::getNextElementSibling()
{
    return ( m_xercescDOMElement->getNextElementSibling() )
           ? new DOMElement( m_xercescDOMElement->getNextElementSibling() )
           : NULL;
}


std::string DOMElement::getNodeName()
{
    char *name = xercesc::XMLString::transcode( m_xercescDOMElement->getNodeName( ) );
    std::string nodeName( name );
    xercesc::XMLString::release( &name );

    return nodeName;
}


iDOMNode *DOMElement::getFirstChild()
{
    return ( m_xercescDOMElement->getFirstChild() ) 
           ? new DOMNode( m_xercescDOMElement->getFirstChild() ) 
           : NULL;
}


iDOMNamedNodeMap *DOMElement::getAttributes()
{
    return ( m_xercescDOMElement->getAttributes() ) 
           ? new DOMNamedNodeMap( m_xercescDOMElement->getAttributes() ) 
           : NULL;
}


bool DOMElement::hasAttributes()
{
    return m_xercescDOMElement->hasAttributes( );
}
