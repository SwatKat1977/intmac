//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef DOMDOCUMENT_H
#define DOMDOCUMENT_H

#include <iDOMDocument.h>
#include <xercesc/dom/DOM.hpp>


class DOMDocument : public iDOMDocument
{
public:
    explicit DOMDocument( xercesc::DOMDocument *doc );

    ~DOMDocument( );

    iDOMElement *getDocumentElement( );

private:
    xercesc::DOMDocument *m_wrapped;
};


#endif
