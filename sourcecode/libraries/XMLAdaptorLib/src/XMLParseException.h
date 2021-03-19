//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef XMLPARSEEXCEPTION_H
#define XMLPARSEEXCEPTION_H

#include <string>


class XMLParseException : public std::exception
{
public:
    XMLParseException( const std::string msg ) : m_msg( msg ) { }

    ~XMLParseException( ) throw( ) { }

    const char *what( ) { return m_msg.c_str(); }

private:
    std::string m_msg;
};


#endif
