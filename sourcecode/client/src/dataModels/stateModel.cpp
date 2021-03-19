//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <dataModels/stateModel.h>


StateModel::StateModel()
          : m_msgHandler( nullptr ) 
          , m_currentUser( "" )
          , m_realm( "" )
          , m_selectedProjectName( "" )
          , m_selectedProjectID( 0 )
{

    m_availableProjectsList.m_listLoaded = false;
    m_availableProjectsList.m_projectsList.clear();
}


StateModel::~StateModel()
{
}


void StateModel::SetMessageHandler( Comms::iMessageHandler *msgHandler )
{
    m_msgHandler = msgHandler;
}


void StateModel::SetUser( std::string username )
{
    m_currentUser = username;
}


void StateModel::SetRealm( std::string realm )
{
    m_realm = realm;
}


void StateModel::SetSelectedProject( ProjectID projectID, std::string project )
{
    m_selectedProjectName = project;
    m_selectedProjectID = projectID;
}


void StateModel::SetHandshakeCompleteStatus( bool status )
{
    m_handshakeComplete = status;
}


void StateModel::AddProjectsList( ProjectsList &projects )
{
    m_availableProjectsList.m_projectsList = projects;
    m_availableProjectsList.m_listLoaded = true;
}


std::string StateModel::GetSelectedProjectName()
{
    return m_selectedProjectName;
}


ProjectID StateModel::GetSelectedProjectID()
{
    return m_selectedProjectID;
}


std::string StateModel::GetUser()
{
    //  NOTE : Will need to add check for authentication status (via triggers)
    return m_currentUser;
}


bool StateModel::GetHandshakeCompleteStatus()
{
    return m_handshakeComplete;
}


std::string StateModel::GetRealm()
{
    return m_realm;
}


AvailableProjectsList StateModel::GetProjectsList()
{
    return m_availableProjectsList;
}