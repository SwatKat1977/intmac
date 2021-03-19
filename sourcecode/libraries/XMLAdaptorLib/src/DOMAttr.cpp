//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <DOMAttr.h>


DOMAttr::DOMAttr(  xercesc::DOMAttr *attr ) : m_wrapper( attr )
{
}


DOMAttr::~DOMAttr()
{
}


std::string DOMAttr::getName()
{
    char *name = xercesc::XMLString::transcode( m_wrapper->getName() );
    std::string nameStr( name );
    xercesc::XMLString::release( &name );
    
    return nameStr;
}


std::string DOMAttr::getValue()
{
    char *value = xercesc::XMLString::transcode( m_wrapper->getValue() );
    std::string valueStr( value );
    xercesc::XMLString::release( &value );
    
    return valueStr;
}
