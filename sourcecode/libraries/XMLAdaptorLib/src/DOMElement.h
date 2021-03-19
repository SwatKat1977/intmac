//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef DOMELEMENT_H
#define DOMELEMENT_H

#include <xercesc/dom/DOM.hpp>
#include <DOMNode.h>
#include <DOMNamedNodeMap.h>
#include <iDOMElement.h>


class DOMElement : public iDOMElement
{
public:
    explicit DOMElement( xercesc::DOMElement *element );

    ~DOMElement( );

    iDOMElement *getFirstElementChild();

    iDOMElement *getNextElementSibling();

    std::string getNodeName();

    iDOMNode *getFirstChild();

    //  Functions introduced in DOM Level 1
    iDOMNamedNodeMap *getAttributes();
 
    //  Functions introduced in DOM Level 2.
    bool hasAttributes ();

private:
    xercesc::DOMElement *m_xercescDOMElement;
};

#endif
