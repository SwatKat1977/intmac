//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <SQLiteAdaptor.h>
#include <database/databaseCommand.h>

DatabaseCommand::DatabaseCommand() 
               : m_dbObject( NULL )
               , m_adptor( NULL )
{
    m_usage = "database <database_file>";
}


DatabaseCommand::~DatabaseCommand()
{
    //  Check it a database was created, if so disconnect it.
    if ( m_dbObject )
    {
        m_adptor->Disconnect( m_dbObject );
    }
}


std::string DatabaseCommand::Execute( ExtendableConsole::ConsoleArguments args
                                    , ExtendableConsole::iConsole *console )
{
    //  Create a database adaptor and then connect to the database.
    m_adptor = new DatabaseAdaptor::SQLiteAdaptor;
    m_dbObject = m_adptor->Connect( args.front(), SQLITE_OPEN_READWRITE );

    //  Check it was created successfully, abort if not!
    if ( m_dbObject->IsValid() == false )
    {
        return std::string( "Unable to connect to database '" +
                            args.front() + "'");
    }

    return "Opened database '" + args.front() + "'";
}
    

bool DatabaseCommand::HasEnoughParameters( unsigned int totalParams )
{
    return ( totalParams == 1 ) ? true : false;
}


DatabaseAdaptor::iDatabaseObject *DatabaseCommand::GetDatabaseObject()
{
    return m_dbObject;
}
