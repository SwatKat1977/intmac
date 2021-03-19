//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <database/queryCommand.h>
#include <database/databaseCommand.h>

using namespace DatabaseAdaptor;


QueryCommand::QueryCommand()
{
    m_usage = "query <sql>";
}


QueryCommand::~QueryCommand()
{
}


std::string QueryCommand::Execute( ExtendableConsole::ConsoleArguments args
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

    if ( dbObject->RawQuery( args.front() ) == false )
    {
        return dbObject->GetErrorMessage();
    }

    iResultsSet *results = dbObject->GetResults();

    if ( results->Size() == 0 )
    {
        return "Total result rows: " +
               std::to_string( results->Size() );
    }

    for ( int i = 0; i < results->Size(); i++ )
    {
        iResultsRow *row = results->GetRow( i );

        //  If first row then display headers before the data
        if ( i == 0 )
        {
            for ( int n = 0; n < row->Size(); n++ )
            {
                console->WriteToConsole( row->GetColumn( n )->GetKey() + "\t" );
            }
            console->WriteToConsole( "\n+-----------------------------+\n" );  
        }
        
        for ( int n = 0; n < row->Size(); n++ )
        {
            console->WriteToConsole( row->GetColumn( n )->GetValue() + "\t" );
        }
        console->WriteToConsole( "\n" );  
    }

    return "Total result rows: " +
           std::to_string( results->Size() );
}
    

bool QueryCommand::HasEnoughParameters( unsigned int totalParams )
{
    return ( totalParams == 1 ) ? true : false;
}
   