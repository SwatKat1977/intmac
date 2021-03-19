//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <database/createCommand.h>
#include <SQLiteAdaptor.h>
#include <databaseTables.h>


CreateCommand::CreateCommand()
{
    m_usage = "create <database_file>";
}


CreateCommand::~CreateCommand()
{
}
   

std::string CreateCommand::Execute( ExtendableConsole::ConsoleArguments args
                                  , ExtendableConsole::iConsole *console )
{
    std::string returnValue;
    DatabaseAdaptor::iDatabaseObject  *dbObject = NULL;
    DatabaseAdaptor::SQLiteAdaptor *adaptor = NULL;
    const int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

    //  Create a database adaptor and then connect ('create') the database.
    adaptor = new DatabaseAdaptor::SQLiteAdaptor;
    dbObject = adaptor->Connect( args.front(), flags );

    //  Check it was created successfully, abort if not!
    if ( dbObject->IsValid() == false )
    {
        return std::string( "Unable to create database '" +
                            args.front() + "'");
    }
    
    //  Create database table 'user'
    std::string tableCreate_user = "CREATE TABLE IF NOT EXISTS user(" +
                                   TableDefinition_user + ")";
    if ( dbObject->RawQuery( tableCreate_user ) == false )
    {
        adaptor->Disconnect( dbObject );
        std::string exceptStr = std::string( "For database '" + args.front() +
                                "', failed to create table 'user'" ); 
        throw std::exception( exceptStr.c_str() );
    }

    //  Create database table 'domain'
    std::string tableCreate_domain = "CREATE TABLE IF NOT EXISTS domain(" +
                                   TableDefinition_domain + ")";
    if ( dbObject->RawQuery( tableCreate_domain ) == false )
    {
        adaptor->Disconnect( dbObject );
        std::string exceptStr = std::string( "For database '" + args.front() +
                                "', failed to create table 'domain'" ); 
        throw std::exception( exceptStr.c_str() );
    }

    //  Create database table 'domainParent'
    std::string tableCreate_domainParent = "CREATE TABLE IF NOT EXISTS domainParent(" +
                                   TableDefinition_domainParent + ")";
    if ( dbObject->RawQuery( tableCreate_domainParent ) == false )
    {
        adaptor->Disconnect( dbObject );
        std::string exceptStr = std::string( "For database '" + args.front() +
                                "', failed to create table 'domainParent'" ); 
        throw std::exception( exceptStr.c_str() );
    }

    //  Create database table 'project'
    std::string tableCreate_project = "CREATE TABLE IF NOT EXISTS project(" +
                                      TableDefinition_project + ")";
    if ( dbObject->RawQuery( tableCreate_project ) == false )
    {
        adaptor->Disconnect( dbObject );
        std::string exceptStr = std::string( "For database '" + args.front() +
                                "', failed to create table 'project'" ); 
        throw std::exception( exceptStr.c_str() );
    }

    //  Create database table 'asset'
    std::string tableCreate_asset = "CREATE TABLE IF NOT EXISTS asset(" +
                                    TableDefinition_asset + ")";
    if ( dbObject->RawQuery( tableCreate_asset ) == false )
    {
        adaptor->Disconnect( dbObject );
        std::string exceptStr = std::string( "For database '" + args.front() +
                                "', failed to create table 'asset'" ); 
        throw std::exception( exceptStr.c_str() );
    }

    //  Create database table 'assetParent'
    std::string tableCreate_assetParent = "CREATE TABLE IF NOT EXISTS assetParent(" +
                                    TableDefinition_assetParent + ")";
    if ( dbObject->RawQuery( tableCreate_assetParent ) == false )
    {
        adaptor->Disconnect( dbObject );
        std::string exceptStr = std::string( "For database '" + args.front() +
                                "', failed to create table 'assetParent'" ); 
        throw std::exception( exceptStr.c_str() );
    }

    //  Create database table 'projectAssetMap'
    std::string tableCreate_projectAssetMap = "CREATE TABLE IF NOT EXISTS projectAssetMap(" +
                                    TableDefinition_projectAssetMap + ")";
    if ( dbObject->RawQuery( tableCreate_projectAssetMap ) == false )
    {
        adaptor->Disconnect( dbObject );
        std::string exceptStr = std::string( "For database '" + args.front() +
                                "', failed to create table 'projectAssetMap'" ); 
        throw std::exception( exceptStr.c_str() );
    }

    //  Create database table 'release'
    std::string tableCreate_release = "CREATE TABLE IF NOT EXISTS release(" +
                                      TableDefinition_release + ")";
    if ( dbObject->RawQuery( tableCreate_release ) == false )
    {
        adaptor->Disconnect( dbObject );
        std::string exceptStr = std::string( "For database '" + args.front() +
                                "', failed to create table 'release'" ); 
        throw std::exception( exceptStr.c_str() );
    }

    //  Create database table 'projectReleaseMap'
    std::string tableCreate_projectReleaseMap = "CREATE TABLE IF NOT EXISTS projectReleaseMap(" +
                                                TableDefinition_projectAssetMap + ")";
    if ( dbObject->RawQuery( tableCreate_projectReleaseMap ) == false )
    {
        adaptor->Disconnect( dbObject );
        std::string exceptStr = std::string( "For database '" + args.front() +
                                "', failed to create table 'tableCreate_projectReleaseMap'" ); 
        throw std::exception( exceptStr.c_str() );
    }

    adaptor->Disconnect( dbObject );

    return std::string( "Database '" + args.front() + "' created" );
}


bool CreateCommand::HasEnoughParameters( unsigned int totalParams )
{
    return ( totalParams == 1 ) ? true : false;
}
