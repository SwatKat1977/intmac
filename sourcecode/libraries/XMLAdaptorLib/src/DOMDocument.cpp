//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <DOMDocument.h>
#include <DOMElement.h>


DOMDocument::DOMDocument( xercesc::DOMDocument *doc )
           : m_wrapped( doc )
{
}


DOMDocument::~DOMDocument( )
{
}

iDOMElement *DOMDocument::getDocumentElement( )
{
    return ( m_wrapped->getDocumentElement( ) == NULL ) ? NULL : 
           new DOMElement( m_wrapped->getDocumentElement( ) ); 
}
