//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>
#include <map>
#include <console/iConsole.h>
#include <console/iConsoleCommand.h>
#include <console/stringIterator.h>


namespace ExtendableConsole
{

typedef std::map<std::string, iConsoleCommand *> CommandsList;

class Console : public iConsole
{
public:
    Console();

    ~Console();

    void WriteToConsole( std::string message );

    void Run();

    void AddCommand( std::string command
                   , iConsoleCommand *commandObject );
    
    bool HasConsoleClosed() { return m_consoleClosed; }

    void SetConsoleClosed( bool state ) { m_consoleClosed = state; }

private:
	CommandsList m_commands;
    bool m_consoleClosed;

    std::string ReadFromConsole( std::string prompt );

	iConsoleCommand *GetCommand( std::string command );

    std::string ExtractCommandToken( StringIterator *cmd );

};

}

#endif
