//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <console/stringIterator.h>

namespace ExtendableConsole
{

StringIterator::StringIterator( std::string str ) 
              : m_str( str ), m_position( 0 )
{
}

 
StringIterator::~StringIterator()
{
}


char StringIterator::GetNext() 
{ 
    char currentCharacter = ' ';

    if ( m_position == m_str.size() ) return EndOfStringChar;

    currentCharacter = m_str[m_position];

    MoveForward();

    return currentCharacter; 
}


char StringIterator::PeekNext() 
{ 
    char currentCharacter = ' ';

    if ( m_position == m_str.size() ) return EndOfStringChar;

    return m_str[m_position];
}


void StringIterator::MoveForward()
{
    //  If position is at the end of the string then don't move the 
    //  position forward as it will be out of range!      
    if ( m_position < m_str.size() ) m_position++;
}

}   //  namespace ExtendableConsole 
