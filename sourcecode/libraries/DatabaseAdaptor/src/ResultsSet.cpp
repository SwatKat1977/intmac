//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <ResultsSet.h>

namespace DatabaseAdaptor
{

ResultsSet::ResultsSet()
{
}


ResultsSet::~ResultsSet()
{
}


void ResultsSet::AddRow( iResultsRow *newRow )
{
    ResultsRow *row = dynamic_cast<ResultsRow *>( newRow );

    m_rows.push_back( *row );
}


iResultsRow *ResultsSet::GetRow( unsigned int rowID )
{
    iResultsRow *row = NULL;

    if ( rowID > ( m_rows.size() - 1 ) )
    {
        row = NULL;
    }
    else
    {
        row = &m_rows[rowID];
    }

    return row;
}


iResultsRow *ResultsSet::First()
{
    if ( m_rows.size() == 0 ) 
    {
        return NULL;
    }
    else
    {
        m_currentItem = m_rows.begin();
        return &*m_currentItem;
    }
}


iResultsRow *ResultsSet::Next()
{
    if ( ( m_rows.size() == 0 ) ||
         ( m_currentItem == m_rows.end() ) )
    {
        return NULL;
    }
    else
    {
        m_currentItem++;
        return ( m_currentItem == m_rows.end() ) ? NULL : &*m_currentItem;
    }
}


iResultsRow *ResultsSet::Last()
{
    if ( m_rows.size() == 0 )
    {
        return NULL;
    }
 
    m_currentItem = m_rows.end() -1;
     
    return &*m_currentItem;
}

void ResultsSet::Clear()
{
    m_rows.clear();
}

}   //  namespace DatabaseAdaptor
