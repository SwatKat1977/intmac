//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef RESULTSROW_H 
#define RESULTSROW_H 

#include <vector>
#include <iResultsRow.h>
#include <ResultColumn.h>

namespace DatabaseAdaptor
{

class ResultsRow : public iResultsRow
{
public:
    ResultsRow();

    ~ResultsRow();

    int Size() { return m_columns.size(); }

    iResultColumn *GetColumn( unsigned int columnID );

    iResultColumn *First();

    iResultColumn *Next();

    iResultColumn *Last();

    void AddColumn( iResultColumn *column );

private:
    std::vector<ResultColumn *> m_columns;
    std::vector<ResultColumn *>::iterator m_currentItem;
};

}   //  namespace DatabaseAdaptor

#endif
