//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef STATEMODEL_H
#define STATEMODEL_H

#include <string>
#include <MVP/iModel.h>
#include <system/iMessageHandler.h>
#include <system/projectConstants.h>


struct AvailableProjectsList
{
    bool m_listLoaded;
    ProjectsList m_projectsList;
};


class StateModel : public iModel
{
public:
    StateModel();

    ~StateModel();

    void SetMessageHandler( Comms::iMessageHandler *msgHandler );

    //  +++++++++++++++++
    //  + Data Setters
    //  +++++++++++++++++
    void SetUser( std::string username );

    void SetRealm( std::string realm );

    void SetSelectedProject( ProjectID projectID, std::string project );

    void SetHandshakeCompleteStatus( bool status );

    void AddProjectsList( ProjectsList &projects );

    //  +++++++++++++++++
    //  + Data Getters
    //  +++++++++++++++++
    std::string GetUser();

    std::string GetRealm();

    std::string GetSelectedProjectName();

    ProjectID GetSelectedProjectID();

    bool GetHandshakeCompleteStatus();

    AvailableProjectsList GetProjectsList();

protected:
    std::string m_currentUser;
    std::string m_realm;
    std::string m_selectedProjectName;
    ProjectID m_selectedProjectID;
    Comms::iMessageHandler *m_msgHandler;
    bool m_handshakeComplete;
    AvailableProjectsList m_availableProjectsList;
};

#endif
