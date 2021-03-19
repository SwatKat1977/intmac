//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IRESULTCOLUMN_H
#define IRESULTCOLUMN_H

#include <string>

namespace DatabaseAdaptor
{

class iResultColumn
{
public:
    virtual ~iResultColumn() {}

    virtual std::string GetKey() = 0;

    virtual std::string GetValue() = 0;
};

}   //  namespace DatabaseAdaptor

#endif
