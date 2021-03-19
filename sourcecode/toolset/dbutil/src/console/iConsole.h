//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef ICONSOLE_H
#define ICONSOLE_H

#include <string>
#include <console/iConsoleCommand.h>

namespace ExtendableConsole
{

class iConsole
{
public:
    virtual ~iConsole() {}

    virtual void WriteToConsole( std::string message ) = 0;

    virtual void Run() = 0;

    virtual void AddCommand( std::string command
                           , iConsoleCommand *commandObject ) = 0;

    virtual bool HasConsoleClosed() = 0;

    virtual void SetConsoleClosed( bool state ) = 0;

	virtual iConsoleCommand *GetCommand( std::string command ) = 0;

protected:
    virtual std::string ReadFromConsole( std::string prompt ) = 0;
};

}   //  namespace ExtendableConsole 


#endif
