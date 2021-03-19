//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IRESULTSSET_H
#define IRESULTSSET_H

#include <iResultsRow.h>

namespace DatabaseAdaptor
{

class iResultsSet
{
public:
    virtual ~iResultsSet() { }

    virtual void AddRow( iResultsRow *newRow ) = 0;

    virtual iResultsRow *GetRow( unsigned int rowID ) = 0;

    virtual int Size() = 0;

    virtual iResultsRow *First() = 0;

    virtual iResultsRow *Next() = 0;

    virtual iResultsRow *Last() = 0;

    virtual void Clear() = 0;
};

}   //  namespace DatabaseAdaptor

#endif
