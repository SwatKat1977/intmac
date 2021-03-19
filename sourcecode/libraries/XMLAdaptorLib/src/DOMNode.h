//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef DOMNODE_H
#define DOMNODE_H

#include <string>
#include <xercesc/dom/DOM.hpp>
#include <iDOMNode.h>


class DOMNode : public iDOMNode
{
public:
    DOMNode( xercesc::DOMNode *node );
    
    ~DOMNode( );

    std::string getNodeName();

    DOMNode *getFirstChild();

    std::string getNodeValue();

private:
    xercesc::DOMNode *m_nodeWrapper;
};


#endif
