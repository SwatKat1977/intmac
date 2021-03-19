//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <ResultsRow.h>

namespace DatabaseAdaptor
{


ResultsRow::ResultsRow()
{
}


ResultsRow::~ResultsRow()
{
}


iResultColumn *ResultsRow::GetColumn( unsigned int columnID )
{
    iResultColumn *column = NULL;

    if ( columnID > ( m_columns.size() - 1 ) )
    {
        column = NULL;
    }
    else
    {
        column = m_columns[columnID];
    }

    return column;
}


iResultColumn *ResultsRow::First()
{
    if ( m_columns.size() == 0 ) 
    {
        return NULL;
    }
    else
    {
        m_currentItem = m_columns.begin();
        return *m_currentItem;
    }
}


iResultColumn *ResultsRow::Next()
{
    if ( ( m_columns.size() == 0 ) ||
         ( m_currentItem == m_columns.end() ) )
    {
        return NULL;
    }
    else
    {
        m_currentItem++;
        return ( m_currentItem == m_columns.end() ) ? NULL : *m_currentItem;
    }
}


iResultColumn *ResultsRow::Last()
{
    if ( m_columns.size() == 0 )
    {
        return NULL;
    }
 
    m_currentItem = m_columns.end() -1;
     
    return *m_currentItem;
}


void ResultsRow::AddColumn( iResultColumn *column )
{
    m_columns.push_back( static_cast< ResultColumn *>( column ) );
}

}   //  namespace DatabaseAdaptor
