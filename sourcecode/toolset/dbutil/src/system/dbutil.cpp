//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <system/dbutils.h>
#include <console/consoleFactory.h>
#include <SQLiteAdaptor.h>

//  Console commands
#include <system/systemCommands.h>
#include <database/createCommand.h>
#include <database/queryCommand.h>
#include <database/databaseCommand.h>
#include <database/importCommand.h>


const std::string appTitle = "INTMAC Database Utility (dbutils) " + 
                             dbutilsVersion + "\n";
const std::string dbAdaptorStr = "Built using the DB Adaptor V" +
                                  DatabaseAdaptor::sqliteAdaptorLibraryVer +
                                  " library\n";
const std::string copyrightStr =
"Copyright 2014-2015 Integrated Test Management Centre Foundation Team\n\
Licensed under the Apache License, Version 2.0 (the \"License\")\n";


int main( int argc, char **argv )
{
    ExtendableConsole::ConsoleFactory *consoleFactory = NULL;
    std::string inputString;

    consoleFactory = new ExtendableConsole::ConsoleFactory;

    ExtendableConsole::iConsole *console = consoleFactory->Create();

    console->WriteToConsole( appTitle );
    console->WriteToConsole( copyrightStr );
    console->WriteToConsole( dbAdaptorStr );

    ExtendableConsole::iConsoleCommand *quitCommand = new QuitCommand;
    ExtendableConsole::iConsoleCommand *helpCommand = new HelpCommand;
    ExtendableConsole::iConsoleCommand *createCommand = new CreateCommand;
    ExtendableConsole::iConsoleCommand *databaseCommand = new DatabaseCommand;
    ExtendableConsole::iConsoleCommand *queryCommand = new QueryCommand;
    ExtendableConsole::iConsoleCommand *importCommand = new ImportCommand;

    console->AddCommand( "quit", quitCommand );
    console->AddCommand( "help", helpCommand );
    console->AddCommand( "create", createCommand );
    console->AddCommand( "database", databaseCommand );
    console->AddCommand( "query", queryCommand );
    console->AddCommand( "import", importCommand );

    while( console->HasConsoleClosed() == false )
    {
        console->Run();
    }

    delete quitCommand;
    delete helpCommand;
    delete createCommand;
    delete databaseCommand;
    delete queryCommand;

    delete console;

    return 0;
}
