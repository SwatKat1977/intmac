//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef DATABASEADAPTOR_H
#define DATABASEADAPTOR_H

#include <string>
#include <iDatabaseAdaptor.h>

namespace DatabaseAdaptor
{

#define SQLITE_OPEN_READONLY        0x00000001
#define SQLITE_OPEN_READWRITE       0x00000002
#define SQLITE_OPEN_CREATE          0x00000004

const std::string sqliteAdaptorLibraryVer = "0.3.2";

class SQLiteAdaptor : public iDatabaseAdaptor
{
public:
    SQLiteAdaptor();

    ~SQLiteAdaptor();

    iDatabaseObject *Connect( std::string databaseFilename, int flags );

    void Disconnect( iDatabaseObject *db );
};

}   //  namespace DatabaseAdaptor

#endif
