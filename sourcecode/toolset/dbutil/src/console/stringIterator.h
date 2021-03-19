//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef STRINGITERATOR_H
#define STRINGITERATOR_H

#include <string>

namespace ExtendableConsole
{

const char EndOfStringChar = -1; 


class StringIterator
{
public:
    StringIterator( std::string str );

    ~StringIterator();

    int GetSize() { return m_str.size(); }
   
    char GetNext();

    char PeekNext();

    void MoveForward();

    unsigned int GetCurrentPosition() { return m_position; }

    std::string GetString() { return m_str; }

private:
    std::string m_str;
    unsigned int m_position;
};

}   //  namespace ExtendableConsole 

#endif
