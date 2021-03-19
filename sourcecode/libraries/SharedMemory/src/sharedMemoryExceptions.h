//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef SHAREDMEMORYEXCEPTIONS_H
#define SHAREDMEMORYEXCEPTIONS_H

#include <IntMacException.h>

namespace Common
{
namespace Shared_Memory
{

const ExceptionID ShMemBaseExceptID = 0x1000;
const ExceptionID ShMemoryExcept_createFail = ShMemBaseExceptID + 1;
const ExceptionID ShMemoryExcept_connectFail = ShMemBaseExceptID + 2;
const ExceptionID ShMemoryExcept_mappingFail = ShMemBaseExceptID + 3;
const ExceptionID ShMemoryExcept_invalidBufferSize = ShMemBaseExceptID + 4;

}
}

#endif
