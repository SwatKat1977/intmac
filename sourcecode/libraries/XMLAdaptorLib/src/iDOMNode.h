//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IDOMNODE_H
#define IDOMNODE_H

#include <string>

class iDOMNode
{
public:    
    virtual ~iDOMNode( ) { }

    virtual std::string getNodeName() =0;

    virtual iDOMNode *getFirstChild() = 0;

    virtual std::string getNodeValue() = 0;
};

#endif
