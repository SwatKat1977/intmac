//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef DATABASECOMMAND_H
#define DATABASECOMMAND_H

#include <iDatabaseAdaptor.h>
#include <SQLiteAdaptor.h>
#include <console/iConsoleCommand.h>

class DatabaseCommand : public ExtendableConsole::iConsoleCommand
{
public:
    DatabaseCommand();

    ~DatabaseCommand();

    std::string Execute( ExtendableConsole::ConsoleArguments args
                       , ExtendableConsole::iConsole *console );
    
    bool HasEnoughParameters( unsigned int totalParams );
    
    std::string GetUsage() { return m_usage; }

    DatabaseAdaptor::iDatabaseObject *GetDatabaseObject();

private:
    std::string m_usage;
    DatabaseAdaptor::iDatabaseObject  *m_dbObject;
    DatabaseAdaptor::SQLiteAdaptor *m_adptor;
};

#endif
