//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IEVENTHANDLER_H
#define IEVENTHANDLER_H

#include <iEventMsg.h>
#include <iEventEnabledObject.h>


class iEventHandler
{
public:
    virtual ~iEventHandler() {}

    virtual void RegisterObserver( iEventEnabledObject *observer ) = 0; 

    virtual void ReceiveMessage( EventID ID
                               , iEventMsg * msg = NULL ) = 0;
};

#endif
