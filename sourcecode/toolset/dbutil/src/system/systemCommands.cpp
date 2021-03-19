//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <system/systemCommands.h>


QuitCommand::QuitCommand()
{
    m_usage = "'QUIT' to exit the application";
}


QuitCommand::~QuitCommand() 
{
}
  

std::string QuitCommand::Execute( ExtendableConsole::ConsoleArguments args
                                , ExtendableConsole::iConsole *console )
{
    console->SetConsoleClosed( true );
    return "";
}


bool QuitCommand::HasEnoughParameters( unsigned int totalParams )
{
    return ( totalParams == 0 ) ? true : false;
}


HelpCommand::HelpCommand()
{
    m_usage = "help <command> to display usage for the command";
}


HelpCommand::~HelpCommand() 
{
}
  

std::string HelpCommand::Execute( ExtendableConsole::ConsoleArguments args
                                , ExtendableConsole::iConsole *console )
{
    iConsoleCommand *cmd = console->GetCommand( args.front() );

    if ( cmd )
    {
        return std::string( cmd->GetUsage() + "\n" );
    }

    return std::string( "Command '" + args.front() + "' is not valid\n" );
}


bool HelpCommand::HasEnoughParameters( unsigned int totalParams )
{
    return ( totalParams == 1 ) ? true : false;
}
