//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <SQLiteAdaptor.h>
#include <SQLiteDatabaseObject.h>


class SQLiteDatabaseObjectTests : public ::testing::Test
{
public:
    SQLiteDatabaseObjectTests() {}

    ~SQLiteDatabaseObjectTests()
    {
        if ( m_SQLiteAdaptor ) delete m_SQLiteAdaptor;
    }

protected:
    DatabaseAdaptor::SQLiteAdaptor *m_SQLiteAdaptor;
};



TEST_F( SQLiteDatabaseObjectTests, RawQuery_InvalidQuery )
{
    m_SQLiteAdaptor = new DatabaseAdaptor::SQLiteAdaptor;
    DatabaseAdaptor::iDatabaseObject  *dbObject = NULL;
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    std::string errMsg = "SQL error: near \"CREATED\": syntax error";

    std::string query;
    
    query = "CREATED TABLE IF NOT EXISTS Example ( id INTEGER PRIMARY KEY )";

    dbObject = m_SQLiteAdaptor->Connect( "queryTesting.db", flags );

    EXPECT_EQ( true, dbObject->IsValid() );

    EXPECT_EQ( false, dbObject->RawQuery( query ) );

    EXPECT_EQ( 0, dbObject->GetErrorMessage().compare( errMsg ) );

    m_SQLiteAdaptor->Disconnect( dbObject );

    EXPECT_EQ( false, dbObject->IsValid() );
}


TEST_F( SQLiteDatabaseObjectTests, RawQuery_CREATE )
{
    m_SQLiteAdaptor = new DatabaseAdaptor::SQLiteAdaptor;
    DatabaseAdaptor::iDatabaseObject  *dbObject = NULL;
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

    std::string query;
    
    query = "CREATE TABLE IF NOT EXISTS Example ( id INTEGER PRIMARY KEY, Route_ID TEXT, Vehicle_Index INTEGER )";

    dbObject = m_SQLiteAdaptor->Connect( "queryTesting.db", flags );

    EXPECT_EQ( true, dbObject->IsValid() );

    EXPECT_EQ( true, dbObject->RawQuery( query ) );

    m_SQLiteAdaptor->Disconnect( dbObject );

    EXPECT_EQ( false, dbObject->IsValid() );
}


TEST_F( SQLiteDatabaseObjectTests, RawQuery_INSERT )
{
    m_SQLiteAdaptor = new DatabaseAdaptor::SQLiteAdaptor;
    DatabaseAdaptor::iDatabaseObject  *dbObject = NULL;
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    std::string query1 = "DELETE FROM Example";
    std::string query2 = "INSERT INTO Example VALUES ( 1, \"Route_ID #1\", 20 )";
    std::string query3 = "INSERT INTO Example VALUES ( 2, \"Route_ID #2\", 20 )";

    dbObject = m_SQLiteAdaptor->Connect( "queryTesting.db", flags );

    EXPECT_EQ( true, dbObject->IsValid() );

    EXPECT_EQ( true, dbObject->RawQuery( query1 ) );
    EXPECT_EQ( true, dbObject->RawQuery( query2 ) );
    EXPECT_EQ( true, dbObject->RawQuery( query3 ) );

    m_SQLiteAdaptor->Disconnect( dbObject );

    EXPECT_EQ( false, dbObject->IsValid() );
}


TEST_F( SQLiteDatabaseObjectTests, RawQuery_SELECT )
{
    m_SQLiteAdaptor = new DatabaseAdaptor::SQLiteAdaptor;
    DatabaseAdaptor::iDatabaseObject  *dbObject = NULL;
    int flags = SQLITE_OPEN_READWRITE;
    std::string query1 = "DELETE FROM Example";
    std::string query2 = "INSERT INTO Example VALUES ( 1, \"Route_ID #1\", 20 )";
    std::string query3 = "INSERT INTO Example VALUES ( 2, \"Route_ID #2\", 20 )";
    std::string query4 = "SELECT * FROM Example";
    DatabaseAdaptor::ResultsSet *set = NULL;

    dbObject = m_SQLiteAdaptor->Connect( "queryTesting.db", flags );

    EXPECT_EQ( true, dbObject->IsValid() );

    EXPECT_EQ( true, dbObject->RawQuery( query1 ) );
    EXPECT_EQ( true, dbObject->RawQuery( query2 ) );
    EXPECT_EQ( true, dbObject->RawQuery( query3 ) );
    EXPECT_EQ( true, dbObject->RawQuery( query4 ) );

    set = dynamic_cast<DatabaseAdaptor::ResultsSet *>( dbObject->GetResults() ); 

    EXPECT_EQ( 2, set->Size() );

    //  ROW 1 validation
    EXPECT_EQ( 0, set->GetRow( 0 )->GetColumn( 0 )->GetKey().compare( "id" ) );
    EXPECT_EQ( 0, set->GetRow( 0 )->GetColumn( 0 )->GetValue().compare( "1" ) );
    EXPECT_EQ( 0, set->GetRow( 0 )->GetColumn( 1 )->GetKey().compare( "Route_ID" ) );
    EXPECT_EQ( 0, set->GetRow( 0 )->GetColumn( 1 )->GetValue().compare( "Route_ID #1" ) );
    EXPECT_EQ( 0, set->GetRow( 0 )->GetColumn( 2 )->GetKey().compare( "Vehicle_Index" ) );
    EXPECT_EQ( 0, set->GetRow( 0 )->GetColumn( 2 )->GetValue().compare( "20" ) );

    //  ROW 2 validation
    EXPECT_EQ( 0, set->GetRow( 1 )->GetColumn( 0 )->GetKey().compare( "id" ) );
    EXPECT_EQ( 0, set->GetRow( 1 )->GetColumn( 0 )->GetValue().compare( "2" ) );
    EXPECT_EQ( 0, set->GetRow( 1 )->GetColumn( 1 )->GetKey().compare( "Route_ID" ) );
    EXPECT_EQ( 0, set->GetRow( 1 )->GetColumn( 1 )->GetValue().compare( "Route_ID #2" ) );
    EXPECT_EQ( 0, set->GetRow( 1 )->GetColumn( 2 )->GetKey().compare( "Vehicle_Index" ) );
    EXPECT_EQ( 0, set->GetRow( 1 )->GetColumn( 2 )->GetValue().compare( "20" ) );

    m_SQLiteAdaptor->Disconnect( dbObject );

    EXPECT_EQ( false, dbObject->IsValid() );
}
