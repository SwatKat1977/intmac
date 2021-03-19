//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <xercesc/sax/ErrorHandler.hpp>
#include <iErrorHandler.h>


enum XMLValidationErrorType
{
    ValErrType_warning,
    ValErrType_error,
    ValErrType_fatalError
};


class ErrorHandler : public xercesc::ErrorHandler
{
public:
    ErrorHandler(  );

    ~ErrorHandler( );

private:
    void warning( const xercesc_3_1::SAXParseException &except );

    void error( const xercesc_3_1::SAXParseException &except );

    void fatalError( const xercesc_3_1::SAXParseException &except );

    void resetErrors();

    void reportParseException( XMLValidationErrorType errType,
                               const xercesc::SAXParseException &except );
};

#endif
