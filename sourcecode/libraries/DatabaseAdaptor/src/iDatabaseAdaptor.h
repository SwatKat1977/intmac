//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IDATABASEADAPTOR_H
#define IDATABASEADAPTOR_H

#include <string>
#include <iDatabaseObject.h>

namespace DatabaseAdaptor
{

class iDatabaseAdaptor
{
public:
    ~iDatabaseAdaptor() { }

    virtual iDatabaseObject *Connect( std::string databaseFilename, int flags ) = 0;

    virtual void Disconnect( iDatabaseObject *db ) = 0;
 };

}   //  namespace DatabaseAdaptor

#endif
