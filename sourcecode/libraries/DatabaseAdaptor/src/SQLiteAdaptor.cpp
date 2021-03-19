//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <sstream>
#include <SQLiteAdaptor.h>
#include <SQLiteDatabaseObject.h>
#include <ResultsRow.h>
#include <ResultColumn.h>


namespace DatabaseAdaptor
{

class SQLiteCallbackWrapper
{
public:
    SQLiteAdaptor *m_adaptor;
    SQLiteDatabaseObject *m_dbObject;
};


SQLiteAdaptor::SQLiteAdaptor()
{
}


SQLiteAdaptor::~SQLiteAdaptor()
{
}


iDatabaseObject *SQLiteAdaptor::Connect( std::string databaseFilename
                                       , int flags )
{
    std::stringstream errorStr;
    int openReturn;
    SQLiteDatabaseObject *dbObject = new SQLiteDatabaseObject;
    sqlite3 *sqlObject = NULL;

    openReturn = sqlite3_open_v2( databaseFilename.c_str()
                                  , &sqlObject
                                  , flags
                                  , NULL );

    if( openReturn )
    {
        errorStr << "Can't open database '" 
                 << databaseFilename
                 << "' reason : "
                 << sqlite3_errmsg( sqlObject );

        dbObject->SetErrorMessage( errorStr.str() );

        dbObject->SetValidity( false );

        sqlite3_close( sqlObject );
    }
    else
    {
        dbObject->SetDatabaseObject( sqlObject );
        dbObject->SetValidity( true );
    }

    return dbObject;
}


void SQLiteAdaptor::Disconnect( iDatabaseObject * db )
{
    sqlite3_close( static_cast<SQLiteDatabaseObject *>(db)->GetDatabaseObject() );

    db->SetValidity( false );
}

}   //  namespace DatabaseAdaptor
