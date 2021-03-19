//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef DOMATTR_H
#define DOMATTR_H


#include <iDOMAttr.h>
#include <xercesc/dom/DOM.hpp>


class DOMAttr : public iDOMAttr
{
public:
    DOMAttr(  xercesc::DOMAttr *m_attr );

    ~DOMAttr( );

    //  Functions introduced in DOM Level 1
    std::string getName();

    std::string getValue();

private:
    xercesc::DOMAttr *m_wrapper;
};

#endif
