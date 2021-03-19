//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <SQLiteDatabaseObject.h>


namespace DatabaseAdaptor
{

//  Forward declaration of SQLite Callback function
int SQLiteCallback( void *obj, int argc, char **argv, char **colName );


SQLiteDatabaseObject::SQLiteDatabaseObject() 
                    : m_databaseObject( NULL )
                    , m_isValid( false )
                    , m_results( new ResultsSet )
{
}


SQLiteDatabaseObject::~SQLiteDatabaseObject()
{
    if ( m_results ) delete m_results;
}


bool SQLiteDatabaseObject::RawQuery( std::string query )
{
    bool returnValue = true;
    char *errorMessage = NULL;
    std::string lastErrorMessage;

    m_results->Clear();

    if ( sqlite3_exec( m_databaseObject,
                       query.c_str(), 
                       SQLiteCallback, 
                       this, 
                       &errorMessage) !=SQLITE_OK )
    {
        lastErrorMessage = "SQL error: ";
        lastErrorMessage.append( errorMessage );
        
        SetErrorMessage( lastErrorMessage );

        sqlite3_free( errorMessage );
        returnValue = false;
    }

    return returnValue;
}


int SQLiteDatabaseObject::SQLiteCallbackFunc( int argc,
                                              char **argv,
                                              char **azColName )
{
    ResultsSet *queryResult = NULL;
    ResultsRow newRow;

    queryResult = GetResults();

    for( int i=0; i < argc; i++ )
    {
        ResultColumn *column = NULL;
        column = new ResultColumn( azColName[i], argv[i] ? argv[i] : "NULL");
        newRow.AddColumn( column );
    }

    queryResult->AddRow( &newRow );

    SetResults( queryResult );

    return 0;
}


int SQLiteCallback( void *obj, int argc, char **argv, char **azColName )
{
    SQLiteDatabaseObject *dbObj = static_cast<SQLiteDatabaseObject *>( obj ); 

    return dbObj->SQLiteCallbackFunc( argc,
                                      argv,
                                      azColName );
}


}   //  namespace DatabaseAdaptor
