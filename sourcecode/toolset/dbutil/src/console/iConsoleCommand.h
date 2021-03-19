//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef ICONSOLECOMMAND_H
#define ICONSOLECOMMAND_H

#include <string>
#include <list>

namespace ExtendableConsole
{

typedef std::list<std::string> ConsoleArguments;

class iConsole;

class iConsoleCommand
{
public:
    virtual ~iConsoleCommand() {}

    virtual bool HasEnoughParameters( unsigned int totalParams ) = 0;

    virtual std::string Execute( ConsoleArguments args
                               , iConsole *console ) = 0;

    virtual std::string GetUsage() = 0;
};

}   //  namespace ExtendableConsole 

#endif
