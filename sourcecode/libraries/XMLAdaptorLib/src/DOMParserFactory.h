//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef DOMPARSERFACTORY_H
#define DOMPARSERFACTORY_H

#include <iDOMParser.h>

class DOMParserFactory
{
public:
    DOMParserFactory( );

    ~DOMParserFactory( );

    iDOMParser *Create( );
};

#endif
