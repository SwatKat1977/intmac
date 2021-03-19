#include <gtest/gtest.h>
#include <ResultsRow.h>
#include <ResultColumn.h>


class ResultsRowTests : public ::testing::Test
{
public:
    ResultsRowTests() {}

    ~ResultsRowTests()
    {
        if ( m_resultsRow ) delete m_resultsRow;
    }

protected:
    DatabaseAdaptor::ResultsRow *m_resultsRow;
};


TEST_F( ResultsRowTests, ObjectCreation_Empty )
{
    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    EXPECT_EQ( 0, m_resultsRow->Size() );
}


TEST_F( ResultsRowTests, Size_Empty )
{
    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    EXPECT_EQ( 0, m_resultsRow->Size() );
}


TEST_F( ResultsRowTests, Size_Populated )
{
    DatabaseAdaptor::ResultColumn column1( "key1", "value1" );
    DatabaseAdaptor::ResultColumn column2( "key2", "value2" );
    DatabaseAdaptor::ResultColumn *retrvColumn1 = NULL;
    DatabaseAdaptor::ResultColumn *retrvColumn2 = NULL;

    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    m_resultsRow->AddColumn( &column1 );
    m_resultsRow->AddColumn( &column2 );

    EXPECT_EQ( 2, m_resultsRow->Size() );

    retrvColumn1 = static_cast<DatabaseAdaptor::ResultColumn *>( m_resultsRow->GetColumn( 0 ) );
    retrvColumn2 = static_cast<DatabaseAdaptor::ResultColumn *>( m_resultsRow->GetColumn( 1 ) );

    EXPECT_EQ( 0, retrvColumn1->GetKey().compare( "key1" ) );
    EXPECT_EQ( 0, retrvColumn1->GetValue().compare( "value1" ) );

    EXPECT_EQ( 0, retrvColumn2->GetKey().compare( "key2" ) );
    EXPECT_EQ( 0, retrvColumn2->GetValue().compare( "value2" ) );

}


TEST_F( ResultsRowTests, First_EmptyList )
{
    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    EXPECT_EQ( 0, m_resultsRow->Size() );

    EXPECT_EQ( NULL, m_resultsRow->First() );
}


TEST_F( ResultsRowTests, First_PopulatedList )
{
    DatabaseAdaptor::ResultColumn column1( "key1", "value1" );
    DatabaseAdaptor::ResultColumn column2( "key2", "value2" );
    DatabaseAdaptor::ResultColumn *firstColumn = NULL;

    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    m_resultsRow->AddColumn( &column1 );
    m_resultsRow->AddColumn( &column2 );

    EXPECT_EQ( 2, m_resultsRow->Size() );

    firstColumn = static_cast<DatabaseAdaptor::ResultColumn *>( m_resultsRow->First() );
    EXPECT_EQ( 0, firstColumn->GetKey().compare( "key1" ) );
    EXPECT_EQ( 0, firstColumn->GetValue().compare( "value1" ) );
}


TEST_F( ResultsRowTests, Next_EmptyList )
{
    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    EXPECT_EQ( 0, m_resultsRow->Size() );

    ASSERT_EQ( NULL, m_resultsRow->First() );

    ASSERT_EQ( NULL, m_resultsRow->Next() );
}


TEST_F( ResultsRowTests, Next_PopulatedMiddleEntry )
{
    DatabaseAdaptor::ResultColumn column1( "key101", "value101" );
    DatabaseAdaptor::ResultColumn column2( "key102", "value102" );
    DatabaseAdaptor::ResultColumn column3( "key103", "value103" );
    DatabaseAdaptor::ResultColumn *firstColumn = NULL;
    DatabaseAdaptor::ResultColumn *middleColumn = NULL;

    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    m_resultsRow->AddColumn( &column1 );
    m_resultsRow->AddColumn( &column2 );
    m_resultsRow->AddColumn( &column3 );

    EXPECT_EQ( 3, m_resultsRow->Size() );

    firstColumn = static_cast<DatabaseAdaptor::ResultColumn *>( m_resultsRow->First() );
    EXPECT_EQ( 0, firstColumn->GetKey().compare( "key101" ) );
    EXPECT_EQ( 0, firstColumn->GetValue().compare( "value101" ) );

    middleColumn = static_cast<DatabaseAdaptor::ResultColumn *>( m_resultsRow->Next() );
    EXPECT_EQ( 0, middleColumn->GetKey().compare( "key102" ) );
    EXPECT_EQ( 0, middleColumn->GetValue().compare( "value102" ) );
}


TEST_F( ResultsRowTests, Next_PopulatedReachEnd )
{
    DatabaseAdaptor::ResultColumn column1( "key101", "value101" );
    DatabaseAdaptor::ResultColumn column2( "key102", "value102" );
    DatabaseAdaptor::ResultColumn *firstColumn = NULL;
    DatabaseAdaptor::ResultColumn *nextColumn = NULL;

    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    m_resultsRow->AddColumn( &column1 );
    m_resultsRow->AddColumn( &column2 );

    EXPECT_EQ( 2, m_resultsRow->Size() );

    firstColumn = static_cast<DatabaseAdaptor::ResultColumn *>( m_resultsRow->First() );
    EXPECT_EQ( 0, firstColumn->GetKey().compare( "key101" ) );
    EXPECT_EQ( 0, firstColumn->GetValue().compare( "value101" ) );

    nextColumn = static_cast<DatabaseAdaptor::ResultColumn *>( m_resultsRow->Next() );
    EXPECT_EQ( 0, nextColumn->GetKey().compare( "key102" ) );
    EXPECT_EQ( 0, nextColumn->GetValue().compare( "value102" ) );

    ASSERT_EQ( NULL, m_resultsRow->Next() );
}


TEST_F( ResultsRowTests, Next_PopulatedAttemptToExceedEnd )
{
    DatabaseAdaptor::ResultColumn column1( "key101", "value101" );
    DatabaseAdaptor::ResultColumn column2( "key102", "value102" );
    DatabaseAdaptor::ResultColumn *firstColumn = NULL;
    DatabaseAdaptor::ResultColumn *nextColumn = NULL;

    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    m_resultsRow->AddColumn( &column1 );
    m_resultsRow->AddColumn( &column2 );

    EXPECT_EQ( 2, m_resultsRow->Size() );

    firstColumn = static_cast<DatabaseAdaptor::ResultColumn *>( m_resultsRow->First() );
    EXPECT_EQ( 0, firstColumn->GetKey().compare( "key101" ) );
    EXPECT_EQ( 0, firstColumn->GetValue().compare( "value101" ) );

    nextColumn = static_cast<DatabaseAdaptor::ResultColumn *>( m_resultsRow->Next() );
    EXPECT_EQ( 0, nextColumn->GetKey().compare( "key102" ) );
    EXPECT_EQ( 0, nextColumn->GetValue().compare( "value102" ) );

    EXPECT_EQ( NULL, m_resultsRow->Next() );
    EXPECT_EQ( NULL, m_resultsRow->Next() );
    EXPECT_EQ( NULL, m_resultsRow->Next() );
}


TEST_F( ResultsRowTests, Last_EmptyList )
{
    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    EXPECT_EQ( 0, m_resultsRow->Size() );

    ASSERT_EQ( NULL, m_resultsRow->Last() );
}


TEST_F( ResultsRowTests, Last_OneEntry )
{
    DatabaseAdaptor::ResultColumn column1( "key101", "value101" );
    DatabaseAdaptor::ResultColumn *lastColumn = NULL;

    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    m_resultsRow->AddColumn( &column1 );

    EXPECT_EQ( 1, m_resultsRow->Size() );

    lastColumn = static_cast<DatabaseAdaptor::ResultColumn *>( m_resultsRow->Last() );
    EXPECT_EQ( 0, lastColumn->GetKey().compare( "key101" ) );
    EXPECT_EQ( 0, lastColumn->GetValue().compare( "value101" ) );
}


TEST_F( ResultsRowTests, Last_PopulatedMultipleEntries )
{
    DatabaseAdaptor::ResultColumn column1( "key101", "value101" );
    DatabaseAdaptor::ResultColumn column2( "key102", "value102" );
    DatabaseAdaptor::ResultColumn column3( "key103", "value103" );
    DatabaseAdaptor::ResultColumn *lastColumn = NULL;

    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    m_resultsRow->AddColumn( &column1 );
    m_resultsRow->AddColumn( &column2 );
    m_resultsRow->AddColumn( &column3 );

    EXPECT_EQ( 3, m_resultsRow->Size() );

    lastColumn = static_cast<DatabaseAdaptor::ResultColumn *>( m_resultsRow->Last() );
    EXPECT_EQ( 0, lastColumn->GetKey().compare( "key103" ) );
    EXPECT_EQ( 0, lastColumn->GetValue().compare( "value103" ) );
}


TEST_F( ResultsRowTests, GetColumn_InvalidColumn )
{
    DatabaseAdaptor::ResultColumn column1( "key101", "value101" );
    DatabaseAdaptor::ResultColumn column2( "key102", "value102" );
    DatabaseAdaptor::ResultColumn column3( "key103", "value103" );

    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    m_resultsRow->AddColumn( &column1 );
    m_resultsRow->AddColumn( &column2 );
    m_resultsRow->AddColumn( &column3 );

    EXPECT_EQ( 3, m_resultsRow->Size() );

    EXPECT_EQ( NULL, m_resultsRow->GetColumn( 3 ) );
}


TEST_F( ResultsRowTests, GetColumn_FirstColumn )
{
    DatabaseAdaptor::ResultColumn column1( "key201", "value401" );
    DatabaseAdaptor::ResultColumn column2( "key202", "value502" );
    DatabaseAdaptor::ResultColumn column3( "key203", "value603" );
    DatabaseAdaptor::ResultColumn *retrievedColumn = NULL;

    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    m_resultsRow->AddColumn( &column1 );
    m_resultsRow->AddColumn( &column2 );
    m_resultsRow->AddColumn( &column3 );

    EXPECT_EQ( 3, m_resultsRow->Size() );

    retrievedColumn = static_cast<DatabaseAdaptor::ResultColumn *>( m_resultsRow->GetColumn( 0 ) );
    EXPECT_EQ( 0, retrievedColumn->GetKey().compare( "key201" ) );
    EXPECT_EQ( 0, retrievedColumn->GetValue().compare( "value401" ) );
}


TEST_F( ResultsRowTests, GetColumn_LastColumn )
{
    DatabaseAdaptor::ResultColumn column1( "key201", "value401" );
    DatabaseAdaptor::ResultColumn column2( "key202", "value502" );
    DatabaseAdaptor::ResultColumn column3( "key203", "value603" );
    DatabaseAdaptor::ResultColumn *retrievedColumn = NULL;

    m_resultsRow = new DatabaseAdaptor::ResultsRow();

    m_resultsRow->AddColumn( &column1 );
    m_resultsRow->AddColumn( &column2 );
    m_resultsRow->AddColumn( &column3 );

    EXPECT_EQ( 3, m_resultsRow->Size() );

    retrievedColumn = static_cast<DatabaseAdaptor::ResultColumn *>( m_resultsRow->GetColumn( 2 ) );
    EXPECT_EQ( 0, retrievedColumn->GetKey().compare( "key203" ) );
    EXPECT_EQ( 0, retrievedColumn->GetValue().compare( "value603" ) );
}
