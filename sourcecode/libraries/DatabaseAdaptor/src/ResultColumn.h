//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef RESULTCOLUMN_H
#define RESULTCOLUMN_H

#include <string>
#include <iResultColumn.h>

namespace DatabaseAdaptor
{

class ResultColumn : public iResultColumn
{
public:
    ResultColumn( std::string key, std::string value ) 
        : m_key( key )
        , m_value( value )
    { }

    ~ResultColumn() {}

    std::string GetKey() { return m_key; }

    std::string GetValue() { return m_value; }

private:
    std::string m_key;
    std::string m_value;
};

}   //  namespace DatabaseAdaptor

#endif
