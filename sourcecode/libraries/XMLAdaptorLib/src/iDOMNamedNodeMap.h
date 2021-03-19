//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IDOMNAMEDNODEMAP_H
#define IDOMNAMEDNODEMAP_H

#include <iDOMAttr.h>


class iDOMNamedNodeMap
{
public:
    virtual ~iDOMNamedNodeMap() { }

    //  Functions introduced in DOM Level 1
    virtual size_t getLength() =0;

    virtual iDOMAttr *item( size_t index ) = 0;
};

#endif
