//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef COMMSTHREAD_H
#define COMMSTHREAD_H

#include <wx/wx.h>
#include <system/iCommunicationsLayer.h>
#include <system/MessageHandler.h>


class CommsThread : public wxThread
{
public:
    CommsThread( iCommunicationsLayer *commsLayer
               , Comms::MessageHandler *msgHandler );

    ~CommsThread();

    wxThread::ExitCode Entry();

protected:
    iCommunicationsLayer *m_commsLayer;
    Comms::MessageHandler *m_msgHandler;
};

#endif
