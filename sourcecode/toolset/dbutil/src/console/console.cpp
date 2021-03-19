//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <console/console.h>


namespace ExtendableConsole
{

const std::string DefaultConsolePrompt = "ready>";


Console::Console() : m_consoleClosed( false )
{
}


Console::~Console()
{
}


void Console::WriteToConsole( std::string message )
{
    if ( message.size() > 0 )
    {
        std::cout << message;
    }
}


std::string Console::ReadFromConsole( std::string prompt )
{       
    std::string inputString;

    std::cout << prompt;

    std::getline( std::cin, inputString );

    return inputString;
}


void Console::Run()
{        
    std::string inputString;
    std::string command;

    inputString = ReadFromConsole( DefaultConsolePrompt );

    //  If nothing has been read then don't process it!
    if ( inputString.size() == 0 ) return;

    StringIterator stringIt( inputString );

    try
    {
        command = ExtractCommandToken( &stringIt );
    }
    catch( std::exception &except)
    {
        WriteToConsole( std::string( except.what() +
                        std::string( "\n" ) ) );
        return;
    }

    iConsoleCommand *cmd = GetCommand( command );

    if ( cmd )
    {
        try
        {
            ConsoleArguments args;
            std::string argStr;

            do
            {
                argStr.clear();
                argStr = ExtractCommandToken( &stringIt );

                if ( argStr.size() ) args.push_back( argStr );
            }
            while ( argStr.size() );

            if ( cmd->HasEnoughParameters( args.size() ) )
            {
                WriteToConsole( std::string( cmd->Execute( args, this ) +
                                "\n" ) );
            }
            else
            {
                WriteToConsole( std::string( "'" + 
                                             command + 
                                             "' has incorrect parameters, " +
                                             "usage is: " +
                                             cmd->GetUsage() + "\n" ) );
            }
        }
        catch( std::exception &except)
        {
            WriteToConsole( std::string( except.what() +
                            std::string( "\n" ) ) );
        }
    }
    else
    {
        WriteToConsole( std::string( "'" + inputString + 
                                     "' is not an recognised command.\n" ) );
    }
}

	
void Console::AddCommand( std::string command
                        , iConsoleCommand *commandObject )
{
    if ( m_commands.find( command ) != m_commands.end() )
    {
        throw std::exception( "Command already added" );
    }

    m_commands[command] = commandObject;
}

	
iConsoleCommand *Console::GetCommand( std::string command )
{
    return ( m_commands.find(command) == m_commands.end() )
           ? NULL : m_commands[command];
}


std::string Console::ExtractCommandToken( StringIterator *cmd )
{
    std::string token;
    char lastCharacter = ' ';

    //  Get the 1st character in the string and then iterator until the current
    //  character is not a space.
    lastCharacter = cmd->GetNext();

    while ( isspace( lastCharacter ) )
    {
        lastCharacter = cmd->GetNext();
    }

    //  Check for quoted text, which must be complete, otherwise an exception
    //  is generated.
    if ( lastCharacter == '"' )
    {
        while ( cmd->PeekNext() != EndOfStringChar &&
                cmd->PeekNext() != '"' )
        {
            lastCharacter = cmd->GetNext();
            token += lastCharacter;
        }
  
        //  Skip over loop terminator, but, we will check it in a minute!
        lastCharacter = cmd->GetNext();

        if ( lastCharacter == EndOfStringChar )
        {
            throw std::exception( "Unmatched quote marks!" );
        }

        if ( isalnum( cmd->PeekNext() ) )
        {
            throw std::exception( "Invalid quote marks!" );
        }
    }
    else if ( ( !isspace( lastCharacter ) ) &&
              ( lastCharacter != EndOfStringChar ) )
    { 
        // identifier: [a-zA-Z][a-zA-Z0-9]*
        token += lastCharacter;

        while ( ( !isspace( cmd->PeekNext() ) ) &&
                ( cmd->PeekNext() != EndOfStringChar ) )
        {
            lastCharacter = cmd->GetNext();
            token += lastCharacter;
        }

        return token;
    }

    return token;
}

}
