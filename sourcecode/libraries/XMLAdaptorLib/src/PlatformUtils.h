//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef PLATFORMUTILS_H
#define PLATFORMUTILS_H

#include <iPlatformUtils.h>


class PlatformUtils : public iPlatformUtils
{
public:

    PlatformUtils();

    ~PlatformUtils();

    void Initialize();

    void Terminate();
};

#endif
