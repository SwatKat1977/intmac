//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <vector>
#include <iEventHandler.h>


typedef std::vector<iEventEnabledObject *> ObserverList;


class EventHandler : public iEventHandler
{
public:
    EventHandler();

    ~EventHandler();

    void RegisterObserver( iEventEnabledObject *observer ); 

    void ReceiveMessage( EventID ID
                       , iEventMsg * msg = NULL );

private:
    ObserverList m_observers;
};

#endif
