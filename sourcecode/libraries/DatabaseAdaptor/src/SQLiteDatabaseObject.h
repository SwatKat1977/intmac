//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef SQLITEDATABASEOBJECT_H
#define SQLITEDATABASEOBJECT_H

#include <sqlite/sqlite3.h>
#include <iDatabaseObject.h>
#include <ResultsSet.h>

namespace DatabaseAdaptor
{

class SQLiteDatabaseObject : public iDatabaseObject
{
public:
    SQLiteDatabaseObject();

    ~SQLiteDatabaseObject();

    sqlite3 *GetDatabaseObject() { return m_databaseObject; }

    void SetDatabaseObject( sqlite3 *db ) { m_databaseObject = db; }

    bool IsValid() { return m_isValid; }

    void SetValidity( bool validity ) { m_isValid = validity; }

    ResultsSet *GetResults() { return m_results; }

    void SetResults( iResultsSet *results) 
    { m_results = dynamic_cast<ResultsSet *>( results ); }

    void SetErrorMessage( std::string msg ) { m_lastError = msg; }

    std::string GetErrorMessage() { return m_lastError; }

    int SQLiteCallbackFunc( int argc,
                            char **argv,
                            char **azColName );

    bool RawQuery( std::string query );

private:
    sqlite3 *m_databaseObject;
    bool m_isValid;
    ResultsSet *m_results;
    std::string m_lastError;
};

}   //  namespace DatabaseAdaptor

#endif
