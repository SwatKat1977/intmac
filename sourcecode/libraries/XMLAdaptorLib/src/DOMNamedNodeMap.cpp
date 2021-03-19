//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include "DOMNamedNodeMap.h"


DOMNamedNodeMap::DOMNamedNodeMap( xercesc::DOMNamedNodeMap *nodeMap ) : m_wrapped( nodeMap )
{
}


DOMNamedNodeMap::~DOMNamedNodeMap( )
{
}


size_t DOMNamedNodeMap::getLength()
{
    return m_wrapped->getLength( );
}


DOMAttr *DOMNamedNodeMap::item( size_t index )
{
    return ( m_wrapped->item( index ) ) 
           ? new DOMAttr( ( xercesc_3_1::DOMAttr * ) m_wrapped->item( index ) ) 
           : NULL;
}
