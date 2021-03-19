//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <PlatformUtils.h>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <XMLParseException.h>


PlatformUtils::PlatformUtils()
{
}


PlatformUtils::~PlatformUtils()
{
}


void PlatformUtils::Initialize()
{
    try
    {
        xercesc::XMLPlatformUtils::Initialize();
    }
    catch ( const xercesc::XMLException &except ) 
    {
        throw XMLParseException( xercesc::XMLString::transcode( except.getMessage( ) ) );
    }
}


void PlatformUtils::Terminate()
{
    xercesc::XMLPlatformUtils::Terminate();
}
