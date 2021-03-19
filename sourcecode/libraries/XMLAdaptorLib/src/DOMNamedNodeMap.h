//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef DOMNAMEDNODEMAP_H
#define DOMNAMEDNODEMAP_H

#include <xercesc/dom/DOM.hpp>
#include <DOMAttr.h>
#include <iDOMNamedNodeMap.h>

class DOMNamedNodeMap : public iDOMNamedNodeMap
{
public:
    DOMNamedNodeMap( xercesc::DOMNamedNodeMap *nodeMap );
     
    ~DOMNamedNodeMap();

    //  Functions introduced in DOM Level 1
    size_t getLength();

    DOMAttr *item( size_t index );

private:
    xercesc::DOMNamedNodeMap *m_wrapped;
};


#endif
