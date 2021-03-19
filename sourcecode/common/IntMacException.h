//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef INTMACEXCEPTION_H
#define INTMACEXCEPTION_H

#include <exception>
#include <string>


typedef unsigned long ExceptionID;


class IntMacException : public std::exception
{
public:
    IntMacException( ExceptionID id, std::string exception )
        : std::exception()
        , m_id( id )
        , m_exception( exception ) { ; }

    ~IntMacException() {}

    ExceptionID GetID() { return m_id; }

    const char* what() { return m_exception.c_str(); }

protected:
    ExceptionID m_id;
    std::string m_exception;
};

#endif
