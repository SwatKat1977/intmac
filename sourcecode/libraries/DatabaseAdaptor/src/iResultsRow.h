//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IRESULTSROW_H 
#define IRESULTSROW_H 

#include <iResultColumn.h>

namespace DatabaseAdaptor
{

class iResultsRow
{
public:
    virtual ~iResultsRow() { }

    virtual int Size() = 0;

    virtual iResultColumn *GetColumn( unsigned int columnID ) = 0;

    virtual iResultColumn *First() = 0;

    virtual iResultColumn *Next() = 0;

    virtual iResultColumn *Last() = 0;

    virtual void AddColumn( iResultColumn *column ) = 0;
};

}   //  namespace DatabaseAdaptor

#endif
