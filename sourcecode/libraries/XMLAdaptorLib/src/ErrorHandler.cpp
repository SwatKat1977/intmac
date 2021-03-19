//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <ErrorHandler.h>
#include <xercesc/sax/SAXParseException.hpp>
#include <sstream>
#include <XMLParseException.h>


ErrorHandler::ErrorHandler( )
{
}


ErrorHandler::~ErrorHandler( )
{
}


void ErrorHandler::warning( const xercesc_3_1::SAXParseException &except )
{
    reportParseException( ValErrType_warning, except );
}


void ErrorHandler::error( const xercesc_3_1::SAXParseException &except )
{
    reportParseException( ValErrType_error, except );
}


void  ErrorHandler::fatalError( const xercesc_3_1::SAXParseException &except )
{
    reportParseException( ValErrType_fatalError, except );
}


void ErrorHandler::resetErrors()
{
}


void ErrorHandler::reportParseException( XMLValidationErrorType errType,
                                         const xercesc_3_1::SAXParseException &ex )
{
    std::stringstream exceptionStr;
    std::string errorTypeStr;
    char *exceptMsg = xercesc_3_1::XMLString::transcode( ex.getMessage() );
    
    switch( errType )
    {
    case ValErrType_warning:
        errorTypeStr = "Warning: ";
        break;

    case ValErrType_error:
        errorTypeStr = "Error: ";
        break;

    case ValErrType_fatalError:
        errorTypeStr = "Fatal Error: ";
        break;
    }

    exceptionStr << errorTypeStr
                 << "XML Schema validation error at line " 
                 << ex.getLineNumber() << ", column " 
                 << ex.getColumnNumber() << ": "
                 << exceptMsg;

    XMLParseException exception( exceptionStr.str( ) );

    xercesc_3_1::XMLString::release( &exceptMsg );

    throw exception;
}
