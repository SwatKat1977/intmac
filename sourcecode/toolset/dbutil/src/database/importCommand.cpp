//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <fstream>
#include <string>
#include <database/importCommand.h>
#include <database/databaseCommand.h>

using namespace DatabaseAdaptor;


ImportCommand::ImportCommand()
{
    m_usage = "import <file_with_path>";
}


ImportCommand::~ImportCommand()
{
}


std::string ImportCommand::Execute( ExtendableConsole::ConsoleArguments args
                                  , ExtendableConsole::iConsole *console )
{
    DatabaseCommand *dbCmd = NULL;

    dbCmd = static_cast<DatabaseCommand *>( console->GetCommand("database") );

    if ( dbCmd == NULL )
    {
        return "Internal error: Required command 'database' not registered!";
    }

    iDatabaseObject *dbObject = dbCmd->GetDatabaseObject();

    if ( !dbObject ) return "No database has been set!";

    std::string importLine;
    std::ifstream importFile( args.front() );

    if ( !importFile.is_open() )
    {
        return "Unable to open import file '" + args.front() + "'";
    }

    int numOfImports = 0;

    while ( getline( importFile, importLine ) )
    {
        if ( dbObject->RawQuery( importLine ) == false )
        {
            console->WriteToConsole( "Database import failed and aborted:\n" );
            return dbObject->GetErrorMessage();
        }

        numOfImports++;
    } 
        
    importFile.close();

    console->WriteToConsole( "File '" + args.front() +
                             "' imported into database.\n" );
    return "Total imported lines: " + std::to_string( numOfImports );
}
    

bool ImportCommand::HasEnoughParameters( unsigned int totalParams )
{
    return ( totalParams == 1 ) ? true : false;
}
   