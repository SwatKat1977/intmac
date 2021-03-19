//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef ICLIENTSERVICE_H
#define ICLIENTSERVICE_H
#include <string>
#include <MsgProtocolDefs.h>


namespace ClientServices
{

class iClientService
{
public:
    virtual ~iClientService() {}

    virtual bool HandleMessage( MessageProtocol::MessageHeader *header
                              , char *body ) = 0;
};

}   //  namespace ClientServices

#endif  //  #ifndef ICLIENTSERVICE_H
