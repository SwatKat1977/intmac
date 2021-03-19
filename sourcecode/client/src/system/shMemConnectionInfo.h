//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef SHMEMCONNECTIONINFO_H
#define SHMEMCONNECTIONINFO_H

#include <string>
#include <system/iCommsLayerConnectInfo.h>


class ShMemConnectionInfo : public iCommsLayerConnectInfo
{
public:
    ShMemConnectionInfo( std::string readBufferName
                       , int readBufferSize
                       , std::string writeBufferName
                       , int writeBufferSize )
                       : m_readBufferName( readBufferName )
                       , m_readBuffersize( readBufferSize )
                       , m_writeBufferName( writeBufferName )
                       , m_writeBufferSize( writeBufferSize )
    { }

    ~ShMemConnectionInfo() {}

    std::string GetReadBufferName() { return m_readBufferName; }

    int GetReadBufferSize() { return m_readBuffersize; }

    std::string GetWriteBufferName() { return m_writeBufferName; }

    int GetWriteBufferSize() { return m_writeBufferSize; }

private:
    std::string m_readBufferName;
    int m_readBuffersize;
    std::string m_writeBufferName;
    int m_writeBufferSize;
};

#endif
