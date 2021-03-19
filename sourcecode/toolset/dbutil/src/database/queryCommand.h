//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef QUERYCOMMAND_H
#define QUERYCOMMAND_H

#include <console/iConsole.h>
#include <console/iConsoleCommand.h>

class QueryCommand : public ExtendableConsole::iConsoleCommand
{
public:
    QueryCommand();

    ~QueryCommand();

    std::string Execute( ExtendableConsole::ConsoleArguments args
                       , ExtendableConsole::iConsole *console );
    
    bool HasEnoughParameters( unsigned int totalParams );
    
    std::string GetUsage() { return m_usage; }

private:
    std::string m_usage;
};

#endif
