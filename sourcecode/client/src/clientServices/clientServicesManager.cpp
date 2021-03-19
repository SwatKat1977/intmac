//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <clientServices\clientServicesManager.h>
#include <clientServices\SessionManagerClientSrv.h>


namespace ClientServices
{


ClientServicesManager::ClientServicesManager( StateModel *stateModel
                                            , ProjectModel *projectModel
                                            , iDOMParser *DOMParser )
    : m_projectModel( projectModel ), m_stateModel( stateModel )
    , m_DOMParser( DOMParser )
{
    m_sessionManagerSrv = new SessionManagerClientSrv( m_stateModel
                                                     , m_projectModel
                                                     , m_DOMParser );
}


ClientServicesManager::~ClientServicesManager()
{
    if ( m_sessionManagerSrv ) delete m_sessionManagerSrv;
}


iSessionManagerClientSrv *ClientServicesManager::GetSessionManagerSrv()
{
    return m_sessionManagerSrv;
}


}   //  namespace ClientServices
