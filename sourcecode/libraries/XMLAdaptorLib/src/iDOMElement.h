//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IDOMELEMENT_H
#define IDOMELEMENT_H

#include <iDOMNode.h>
#include <iDOMNamedNodeMap.h>


class iDOMElement
{
public:
    virtual ~iDOMElement() { } 

    virtual iDOMElement *getFirstElementChild() = 0;

    virtual iDOMElement *getNextElementSibling() = 0;

    virtual std::string getNodeName() = 0;

    virtual iDOMNode *getFirstChild() = 0;

    //  Functions introduced in DOM Level 1
    virtual iDOMNamedNodeMap *getAttributes() = 0;
 
    //  Functions introduced in DOM Level 2.
    virtual bool hasAttributes () = 0;
};


#endif
