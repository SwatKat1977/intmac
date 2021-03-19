//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef RESULTSSET_H
#define RESULTSSET_H

#include <vector>
#include <ResultsRow.h>
#include <iResultsSet.h>
#include <ResultsSet.h>

namespace DatabaseAdaptor
{

class ResultsSet : public iResultsSet
{
public:
    ResultsSet();

    ~ResultsSet();

    void AddRow( iResultsRow *newRow );

    iResultsRow *GetRow( unsigned int rowID );

    int Size() { return m_rows.size(); }

    iResultsRow *First();

    iResultsRow *Next();

    iResultsRow *Last();

    void Clear();
private:
    std::vector<ResultsRow> m_rows;
    std::vector<ResultsRow>::iterator m_currentItem;
};

}   //  namespace DatabaseAdaptor

#endif
