//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <system/eventHandler.h>


EventHandler::EventHandler()
{
}


EventHandler::~EventHandler()
{
}

    
void EventHandler::RegisterObserver( iEventEnabledObject *observer )
{
    m_observers.push_back( observer );

}


void EventHandler::ReceiveMessage( EventID ID
                                 , iEventMsg * msg )
{
    ObserverList::iterator it;

    for( it = m_observers.begin(); it != m_observers.end(); it++ )
    {
        (*it)->ReceiveEventMessage( ID, msg );
    }
}
