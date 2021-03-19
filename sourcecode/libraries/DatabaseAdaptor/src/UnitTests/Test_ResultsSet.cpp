#include <gtest/gtest.h>
#include <ResultsSet.h>
#include <ResultsRow.h>
#include <ResultColumn.h>

class ResultsSetTests : public ::testing::Test
{
public:
    ResultsSetTests() {}

    ~ResultsSetTests()
    {
        if ( m_resultsSet ) delete m_resultsSet;
    }

protected:
    DatabaseAdaptor::ResultsSet *m_resultsSet;
};


TEST_F( ResultsSetTests, ObjectCreation_Empty )
{
    m_resultsSet = new DatabaseAdaptor::ResultsSet();

    EXPECT_EQ( 0, m_resultsSet->Size() );
}


TEST_F( ResultsSetTests, Size_Empty )
{
    m_resultsSet = new DatabaseAdaptor::ResultsSet();

    EXPECT_EQ( 0, m_resultsSet->Size() );
}


TEST_F( ResultsSetTests, Size_Populated )
{
    DatabaseAdaptor::ResultsRow row1;
    DatabaseAdaptor::ResultsRow row2;
    DatabaseAdaptor::ResultColumn row1Column1( "Row1Col1_key", "Row1Col1_value" );
    DatabaseAdaptor::ResultColumn row1Column2( "Row1Col2_key", "Row1Col2_value" );
    DatabaseAdaptor::ResultColumn row2Column1( "Row2Col1_key", "Row2Col1_value" );
    DatabaseAdaptor::ResultColumn row2Column2( "Row2Col2_key", "Row2Col2_value" );
    DatabaseAdaptor::ResultsRow *retrvRow1 = NULL;
    DatabaseAdaptor::ResultsRow *retrvRow2 = NULL;

    row1.AddColumn( &row1Column1 );
    row1.AddColumn( &row1Column2 );
    row2.AddColumn( &row2Column1 );
    row2.AddColumn( &row2Column2 );

    m_resultsSet = new DatabaseAdaptor::ResultsSet();

    m_resultsSet->AddRow( &row1 );
    m_resultsSet->AddRow( &row2 );

    EXPECT_EQ( 2, m_resultsSet->Size() );

    retrvRow1 = static_cast<DatabaseAdaptor::ResultsRow *>( m_resultsSet->GetRow( 0 ) );
    retrvRow2 = static_cast<DatabaseAdaptor::ResultsRow *>( m_resultsSet->GetRow( 1 ) );

    EXPECT_EQ( 0, retrvRow1->GetColumn( 0 )->GetKey().compare( "Row1Col1_key" ) );
    EXPECT_EQ( 0, retrvRow1->GetColumn( 0 )->GetValue().compare( "Row1Col1_value" ) );
    EXPECT_EQ( 0, retrvRow1->GetColumn( 1 )->GetKey().compare( "Row1Col2_key" ) );
    EXPECT_EQ( 0, retrvRow1->GetColumn( 1 )->GetValue().compare( "Row1Col2_value" ) );

    EXPECT_EQ( 0, retrvRow2->GetColumn( 0 )->GetKey().compare( "Row2Col1_key" ) );
    EXPECT_EQ( 0, retrvRow2->GetColumn( 0 )->GetValue().compare( "Row2Col1_value" ) );
    EXPECT_EQ( 0, retrvRow2->GetColumn( 1 )->GetKey().compare( "Row2Col2_key" ) );
    EXPECT_EQ( 0, retrvRow2->GetColumn( 1 )->GetValue().compare( "Row2Col2_value" ) );
}


TEST_F( ResultsSetTests, First_EmptyList )
{
    m_resultsSet = new DatabaseAdaptor::ResultsSet();

    EXPECT_EQ( 0, m_resultsSet->Size() );

    EXPECT_EQ( NULL, m_resultsSet->First() );
}


TEST_F( ResultsSetTests, First_PopulatedList )
{
    DatabaseAdaptor::ResultsRow row1;
    DatabaseAdaptor::ResultsRow row2;
    DatabaseAdaptor::ResultColumn row1Column1( "Row1Col1_key", "Row1Col1_value" );
    DatabaseAdaptor::ResultColumn row1Column2( "Row1Col2_key", "Row1Col2_value" );
    DatabaseAdaptor::ResultColumn row2Column1( "Row2Col1_key", "Row2Col1_value" );
    DatabaseAdaptor::ResultColumn row2Column2( "Row2Col2_key", "Row2Col2_value" );
    DatabaseAdaptor::ResultsRow *retrievedRow = NULL;

    row1.AddColumn( &row1Column1 );
    row1.AddColumn( &row1Column2 );
    row2.AddColumn( &row2Column1 );
    row2.AddColumn( &row2Column2 );

    m_resultsSet = new DatabaseAdaptor::ResultsSet();

    m_resultsSet->AddRow( &row1 );
    m_resultsSet->AddRow( &row2 );

    EXPECT_EQ( 2, m_resultsSet->Size() );

    retrievedRow = static_cast<DatabaseAdaptor::ResultsRow *>( m_resultsSet->First() );

    EXPECT_EQ( 0, retrievedRow->GetColumn( 0 )->GetKey().compare( "Row1Col1_key" ) );
    EXPECT_EQ( 0, retrievedRow->GetColumn( 0 )->GetValue().compare( "Row1Col1_value" ) );
    EXPECT_EQ( 0, retrievedRow->GetColumn( 1 )->GetKey().compare( "Row1Col2_key" ) );
    EXPECT_EQ( 0, retrievedRow->GetColumn( 1 )->GetValue().compare( "Row1Col2_value" ) );
}


TEST_F( ResultsSetTests, Next_EmptyList )
{
    m_resultsSet = new DatabaseAdaptor::ResultsSet();

    EXPECT_EQ( 0, m_resultsSet->Size() );

    ASSERT_EQ( NULL, m_resultsSet->First() );

    ASSERT_EQ( NULL, m_resultsSet->Next() );
}


TEST_F( ResultsSetTests, Next_PopulatedMiddleEntry )
{
    DatabaseAdaptor::ResultsRow row1;
    DatabaseAdaptor::ResultsRow row2;
    DatabaseAdaptor::ResultColumn row1Column1( "Row1Col1_key", "Row1Col1_value" );
    DatabaseAdaptor::ResultColumn row1Column2( "Row1Col2_key", "Row1Col2_value" );
    DatabaseAdaptor::ResultColumn row2Column1( "Row2Col1_key", "Row2Col1_value" );
    DatabaseAdaptor::ResultColumn row2Column2( "Row2Col2_key", "Row2Col2_value" );
    DatabaseAdaptor::ResultsRow *firstRetrievedRow = NULL;
    DatabaseAdaptor::ResultsRow *nextRetrievedRow = NULL;

    row1.AddColumn( &row1Column1 );
    row1.AddColumn( &row1Column2 );
    row2.AddColumn( &row2Column1 );
    row2.AddColumn( &row2Column2 );

    m_resultsSet = new DatabaseAdaptor::ResultsSet();

    m_resultsSet->AddRow( &row1 );
    m_resultsSet->AddRow( &row2 );

    EXPECT_EQ( 2, m_resultsSet->Size() );

    firstRetrievedRow = static_cast<DatabaseAdaptor::ResultsRow *>( m_resultsSet->First() );
    EXPECT_EQ( 0, firstRetrievedRow->GetColumn( 0 )->GetKey().compare( "Row1Col1_key" ) );
    EXPECT_EQ( 0, firstRetrievedRow->GetColumn( 0 )->GetValue().compare( "Row1Col1_value" ) );
    EXPECT_EQ( 0, firstRetrievedRow->GetColumn( 1 )->GetKey().compare( "Row1Col2_key" ) );
    EXPECT_EQ( 0, firstRetrievedRow->GetColumn( 1 )->GetValue().compare( "Row1Col2_value" ) );

    nextRetrievedRow = static_cast<DatabaseAdaptor::ResultsRow *>( m_resultsSet->Next() );
    EXPECT_EQ( 0, nextRetrievedRow->GetColumn( 0 )->GetKey().compare( "Row2Col1_key" ) );
    EXPECT_EQ( 0, nextRetrievedRow->GetColumn( 0 )->GetValue().compare( "Row2Col1_value" ) );
    EXPECT_EQ( 0, nextRetrievedRow->GetColumn( 1 )->GetKey().compare( "Row2Col2_key" ) );
    EXPECT_EQ( 0, nextRetrievedRow->GetColumn( 1 )->GetValue().compare( "Row2Col2_value" ) );
}


TEST_F( ResultsSetTests, Next_PopulatedReachEnd )
{
    DatabaseAdaptor::ResultsRow row1;
    DatabaseAdaptor::ResultsRow row2;
    DatabaseAdaptor::ResultsRow row3;
    DatabaseAdaptor::ResultColumn row1Column1( "Row1Col1_key", "Row1Col1_value" );
    DatabaseAdaptor::ResultColumn row1Column2( "Row1Col2_key", "Row1Col2_value" );
    DatabaseAdaptor::ResultColumn row2Column1( "Row2Col1_key", "Row2Col1_value" );
    DatabaseAdaptor::ResultColumn row2Column2( "Row2Col2_key", "Row2Col2_value" );
    DatabaseAdaptor::ResultColumn row3Column1( "Row3Col1_key", "Row3Col1_value" );
    DatabaseAdaptor::ResultsRow *firstRetrievedRow = NULL;
    DatabaseAdaptor::ResultsRow *nextRetrievedRow = NULL;

    row1.AddColumn( &row1Column1 );
    row1.AddColumn( &row1Column2 );
    row2.AddColumn( &row2Column1 );
    row2.AddColumn( &row2Column2 );
    row3.AddColumn( &row3Column1 );

    m_resultsSet = new DatabaseAdaptor::ResultsSet();

    m_resultsSet->AddRow( &row1 );
    m_resultsSet->AddRow( &row2 );
    m_resultsSet->AddRow( &row3 );

    EXPECT_EQ( 3, m_resultsSet->Size() );

    firstRetrievedRow = static_cast<DatabaseAdaptor::ResultsRow *>( m_resultsSet->First() );
    EXPECT_EQ( 0, firstRetrievedRow->GetColumn( 0 )->GetKey().compare( "Row1Col1_key" ) );
    EXPECT_EQ( 0, firstRetrievedRow->GetColumn( 0 )->GetValue().compare( "Row1Col1_value" ) );
    EXPECT_EQ( 0, firstRetrievedRow->GetColumn( 1 )->GetKey().compare( "Row1Col2_key" ) );
    EXPECT_EQ( 0, firstRetrievedRow->GetColumn( 1 )->GetValue().compare( "Row1Col2_value" ) );

    nextRetrievedRow = static_cast<DatabaseAdaptor::ResultsRow *>( m_resultsSet->Next() );
    nextRetrievedRow = static_cast<DatabaseAdaptor::ResultsRow *>( m_resultsSet->Next() );
    EXPECT_EQ( 0, nextRetrievedRow->GetColumn( 0 )->GetKey().compare( "Row3Col1_key" ) );
    EXPECT_EQ( 0, nextRetrievedRow->GetColumn( 0 )->GetValue().compare( "Row3Col1_value" ) );
}


TEST_F( ResultsSetTests, Next_PopulatedAttemptToExceedEnd )
{
    DatabaseAdaptor::ResultsRow row1;
    DatabaseAdaptor::ResultsRow row2;
    DatabaseAdaptor::ResultsRow row3;
    DatabaseAdaptor::ResultColumn row1Column1( "Row1Col1_key", "Row1Col1_value" );
    DatabaseAdaptor::ResultColumn row1Column2( "Row1Col2_key", "Row1Col2_value" );
    DatabaseAdaptor::ResultColumn row2Column1( "Row2Col1_key", "Row2Col1_value" );
    DatabaseAdaptor::ResultColumn row2Column2( "Row2Col2_key", "Row2Col2_value" );
    DatabaseAdaptor::ResultColumn row3Column1( "Row3Col1_key", "Row3Col1_value" );
    DatabaseAdaptor::ResultsRow *firstRetrievedRow = NULL;
    DatabaseAdaptor::ResultsRow *nextRetrievedRow = NULL;

    row1.AddColumn( &row1Column1 );
    row1.AddColumn( &row1Column2 );
    row2.AddColumn( &row2Column1 );
    row2.AddColumn( &row2Column2 );
    row3.AddColumn( &row3Column1 );

    m_resultsSet = new DatabaseAdaptor::ResultsSet();

    m_resultsSet->AddRow( &row1 );
    m_resultsSet->AddRow( &row2 );
    m_resultsSet->AddRow( &row3 );

    EXPECT_EQ( 3, m_resultsSet->Size() );

    firstRetrievedRow = static_cast<DatabaseAdaptor::ResultsRow *>( m_resultsSet->First() );
    EXPECT_EQ( 0, firstRetrievedRow->GetColumn( 0 )->GetKey().compare( "Row1Col1_key" ) );
    EXPECT_EQ( 0, firstRetrievedRow->GetColumn( 0 )->GetValue().compare( "Row1Col1_value" ) );
    EXPECT_EQ( 0, firstRetrievedRow->GetColumn( 1 )->GetKey().compare( "Row1Col2_key" ) );
    EXPECT_EQ( 0, firstRetrievedRow->GetColumn( 1 )->GetValue().compare( "Row1Col2_value" ) );

    nextRetrievedRow = static_cast<DatabaseAdaptor::ResultsRow *>( m_resultsSet->Next() );
    EXPECT_EQ( 0, nextRetrievedRow->GetColumn( 0 )->GetKey().compare( "Row2Col1_key" ) );
    EXPECT_EQ( 0, nextRetrievedRow->GetColumn( 0 )->GetValue().compare( "Row2Col1_value" ) );
    EXPECT_EQ( 0, nextRetrievedRow->GetColumn( 1 )->GetKey().compare( "Row2Col2_key" ) );
    EXPECT_EQ( 0, nextRetrievedRow->GetColumn( 1 )->GetValue().compare( "Row2Col2_value" ) );

    nextRetrievedRow = static_cast<DatabaseAdaptor::ResultsRow *>( m_resultsSet->Next() );
    EXPECT_EQ( 0, nextRetrievedRow->GetColumn( 0 )->GetKey().compare( "Row3Col1_key" ) );
    EXPECT_EQ( 0, nextRetrievedRow->GetColumn( 0 )->GetValue().compare( "Row3Col1_value" ) );

    EXPECT_EQ( NULL, m_resultsSet->Next() );
    EXPECT_EQ( NULL, m_resultsSet->Next() );
}


TEST_F( ResultsSetTests, Last_EmptyList )
{
    m_resultsSet = new DatabaseAdaptor::ResultsSet();

    EXPECT_EQ( 0, m_resultsSet->Size() );

    ASSERT_EQ( NULL, m_resultsSet->Last() );
}


TEST_F( ResultsSetTests, Last_OneEntry )
{
    DatabaseAdaptor::ResultsRow row1;
    DatabaseAdaptor::ResultColumn row1Column1( "Row1Col1_key", "Row1Col1_value" );
    DatabaseAdaptor::ResultColumn row1Column2( "Row1Col2_key", "Row1Col2_value" );
    DatabaseAdaptor::ResultsRow *retrievedRow = NULL;

    row1.AddColumn( &row1Column1 );
    row1.AddColumn( &row1Column2 );

    m_resultsSet = new DatabaseAdaptor::ResultsSet();

    m_resultsSet->AddRow( &row1 );

    EXPECT_EQ( 1, m_resultsSet->Size() );

    retrievedRow = static_cast<DatabaseAdaptor::ResultsRow *>( m_resultsSet->Last() );
    EXPECT_EQ( 0, retrievedRow->GetColumn( 0 )->GetKey().compare( "Row1Col1_key" ) );
    EXPECT_EQ( 0, retrievedRow->GetColumn( 0 )->GetValue().compare( "Row1Col1_value" ) );
    EXPECT_EQ( 0, retrievedRow->GetColumn( 1 )->GetKey().compare( "Row1Col2_key" ) );
    EXPECT_EQ( 0, retrievedRow->GetColumn( 1 )->GetValue().compare( "Row1Col2_value" ) );
}


TEST_F( ResultsSetTests, Last_PopulatedMultipleEntries )
{
    DatabaseAdaptor::ResultsRow row1;
    DatabaseAdaptor::ResultsRow row2;
    DatabaseAdaptor::ResultsRow row3;
    DatabaseAdaptor::ResultColumn row1Column1( "Row1Col1_key", "Row1Col1_value" );
    DatabaseAdaptor::ResultColumn row1Column2( "Row1Col2_key", "Row1Col2_value" );
    DatabaseAdaptor::ResultColumn row2Column1( "Row2Col1_key", "Row2Col1_value" );
    DatabaseAdaptor::ResultColumn row2Column2( "Row2Col2_key", "Row2Col2_value" );
    DatabaseAdaptor::ResultColumn row3Column1( "Row3Col1_key", "Row3Col1_value" );
    DatabaseAdaptor::ResultsRow *retrievedRow = NULL;

    row1.AddColumn( &row1Column1 );
    row1.AddColumn( &row1Column2 );
    row2.AddColumn( &row2Column1 );
    row2.AddColumn( &row2Column2 );
    row3.AddColumn( &row3Column1 );

    m_resultsSet = new DatabaseAdaptor::ResultsSet();

    m_resultsSet->AddRow( &row1 );
    m_resultsSet->AddRow( &row2 );
    m_resultsSet->AddRow( &row3 );

    EXPECT_EQ( 3, m_resultsSet->Size() );

    retrievedRow = static_cast<DatabaseAdaptor::ResultsRow *>( m_resultsSet->Last() );
    EXPECT_EQ( 0, retrievedRow->GetColumn( 0 )->GetKey().compare( "Row3Col1_key" ) );
    EXPECT_EQ( 0, retrievedRow->GetColumn( 0 )->GetValue().compare( "Row3Col1_value" ) );
}


TEST_F( ResultsSetTests, GetRow_InvalidColumn )
{
    DatabaseAdaptor::ResultsRow row1;
    DatabaseAdaptor::ResultsRow row2;
    DatabaseAdaptor::ResultsRow row3;
    DatabaseAdaptor::ResultColumn row1Column1( "Row1Col1_key", "Row1Col1_value" );
    DatabaseAdaptor::ResultColumn row1Column2( "Row1Col2_key", "Row1Col2_value" );
    DatabaseAdaptor::ResultColumn row2Column1( "Row2Col1_key", "Row2Col1_value" );
    DatabaseAdaptor::ResultColumn row2Column2( "Row2Col2_key", "Row2Col2_value" );
    DatabaseAdaptor::ResultColumn row3Column1( "Row3Col1_key", "Row3Col1_value" );

    row1.AddColumn( &row1Column1 );
    row1.AddColumn( &row1Column2 );
    row2.AddColumn( &row2Column1 );
    row2.AddColumn( &row2Column2 );
    row3.AddColumn( &row3Column1 );

    m_resultsSet = new DatabaseAdaptor::ResultsSet();

    m_resultsSet->AddRow( &row1 );
    m_resultsSet->AddRow( &row2 );
    m_resultsSet->AddRow( &row3 );

    EXPECT_EQ( 3, m_resultsSet->Size() );

    EXPECT_EQ( NULL, m_resultsSet->GetRow( 3 ) );
}


TEST_F( ResultsSetTests, GetRow_ValidColumn )
{
    DatabaseAdaptor::ResultsRow row1;
    DatabaseAdaptor::ResultsRow row2;
    DatabaseAdaptor::ResultsRow row3;
    DatabaseAdaptor::ResultColumn row1Column1( "Row1Col1_key", "Row1Col1_value" );
    DatabaseAdaptor::ResultColumn row1Column2( "Row1Col2_key", "Row1Col2_value" );
    DatabaseAdaptor::ResultColumn row2Column1( "Row2Col1_key", "Row2Col1_value" );
    DatabaseAdaptor::ResultColumn row2Column2( "Row2Col2_key", "Row2Col2_value" );
    DatabaseAdaptor::ResultColumn row3Column1( "Row3Col1_key", "Row3Col1_value" );
    DatabaseAdaptor::ResultsRow *retrievedRow = NULL;

    row1.AddColumn( &row1Column1 );
    row1.AddColumn( &row1Column2 );
    row2.AddColumn( &row2Column1 );
    row2.AddColumn( &row2Column2 );
    row3.AddColumn( &row3Column1 );

    m_resultsSet = new DatabaseAdaptor::ResultsSet();

    m_resultsSet->AddRow( &row1 );
    m_resultsSet->AddRow( &row2 );
    m_resultsSet->AddRow( &row3 );

    EXPECT_EQ( 3, m_resultsSet->Size() );

    retrievedRow = static_cast<DatabaseAdaptor::ResultsRow *>( m_resultsSet->GetRow( 1 ) );
    EXPECT_EQ( 0, retrievedRow->GetColumn( 0 )->GetKey().compare( "Row2Col1_key" ) );
    EXPECT_EQ( 0, retrievedRow->GetColumn( 0 )->GetValue().compare( "Row2Col1_value" ) );
}
