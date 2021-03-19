#include <gtest/gtest.h>
#include <SQLiteAdaptor.h>
#include <SQLiteDatabaseObject.h>


class SQLiteAdaptorTests : public ::testing::Test
{
public:
    SQLiteAdaptorTests() {}

    ~SQLiteAdaptorTests()
    {
        if ( m_SQLiteAdaptor ) delete m_SQLiteAdaptor;
    }

protected:
    DatabaseAdaptor::SQLiteAdaptor *m_SQLiteAdaptor;
};


TEST_F( SQLiteAdaptorTests, ObjectCreation_Empty )
{
    m_SQLiteAdaptor = new DatabaseAdaptor::SQLiteAdaptor;
}


TEST_F( SQLiteAdaptorTests, Connect_DatabaseNotExisting )
{
    m_SQLiteAdaptor = new DatabaseAdaptor::SQLiteAdaptor;
    DatabaseAdaptor::iDatabaseObject  *dbObject = NULL;
    std::string errMsg = "Can't open database 'invalid.db' reason : unable to open database file";

    dbObject = m_SQLiteAdaptor->Connect( "invalid.db", 
                                         SQLITE_OPEN_READWRITE );

    EXPECT_EQ( false, dbObject->IsValid() );

    EXPECT_EQ( 0, dbObject->GetErrorMessage().compare( errMsg ) );
}


TEST_F( SQLiteAdaptorTests, Connect_CreateDatabase )
{
    m_SQLiteAdaptor = new DatabaseAdaptor::SQLiteAdaptor;
    DatabaseAdaptor::iDatabaseObject  *dbObject = NULL;
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

    dbObject = m_SQLiteAdaptor->Connect( "newlycreated.db", flags );

    EXPECT_EQ( true, dbObject->IsValid() );

    delete dbObject;
}


TEST_F( SQLiteAdaptorTests, Disconnect_CheckValidity )
{
    m_SQLiteAdaptor = new DatabaseAdaptor::SQLiteAdaptor;
    DatabaseAdaptor::iDatabaseObject  *dbObject = NULL;
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

    dbObject = m_SQLiteAdaptor->Connect( "newlycreated.db", flags );

    EXPECT_EQ( true, dbObject->IsValid() );

    m_SQLiteAdaptor->Disconnect( dbObject );

    EXPECT_EQ( false, dbObject->IsValid() );
}
