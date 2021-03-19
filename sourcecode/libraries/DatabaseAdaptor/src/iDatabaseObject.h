//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IDATABASEOBJECT_H
#define IDATABASEOBJECT_H

#include <iResultsSet.h>

namespace DatabaseAdaptor
{

class iDatabaseObject
{
public:
    virtual ~iDatabaseObject() {}

     virtual bool IsValid() = 0;

     virtual void SetValidity( bool validity ) = 0;

     virtual iResultsSet *GetResults() = 0;

    virtual void SetResults( iResultsSet * results) = 0;

    virtual void SetErrorMessage( std::string msg ) = 0;

    virtual std::string GetErrorMessage() = 0;

    virtual bool RawQuery( std::string query ) = 0;
};

}   //  namespace DatabaseAdaptor

#endif
