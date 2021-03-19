//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef ISHAREDMEMORYBLOCK_H
#define ISHAREDMEMORYBLOCK_H

namespace Common
{
namespace Shared_Memory
{


class iSharedMemoryBlock
{
public:
    virtual ~iSharedMemoryBlock() {}

    virtual void Read( unsigned int size
                     , unsigned int offset
                     , char *readBuffer ) = 0;

    virtual void Write( char *data
                      , unsigned int size
                      , unsigned int offset ) = 0;

    virtual void Destroy() = 0;
};

}
}

#endif
