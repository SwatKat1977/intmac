#include <gtest/gtest.h>
#include <ResultColumn.h>


class ResultColumnTests : public ::testing::Test
{
public:
    ResultColumnTests() {}

    ~ResultColumnTests()
    {
        if ( m_resultColumn ) delete m_resultColumn;
    }

protected:
    DatabaseAdaptor::ResultColumn *m_resultColumn;
};


TEST_F( ResultColumnTests, ObjectCreation_Empty )
{
    m_resultColumn = new DatabaseAdaptor::ResultColumn( "MyKey", "MyValue" );

    EXPECT_EQ( 0, m_resultColumn->GetKey().compare( "MyKey" ) );
    EXPECT_EQ( 0, m_resultColumn->GetValue().compare( "MyValue" ) );
}
