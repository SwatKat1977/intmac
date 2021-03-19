//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef CLIENTSERVICESMANAGER_H
#define CLIENTSERVICESMANAGER_H
#include <clientServices\iClientServicesManager.h>
#include <dataModels\stateModel.h>
#include <dataModels\projectModel.h>
#include <iDOMParser.h>


namespace ClientServices
{

class ClientServicesManager : public iClientServicesManager
{
public:
    ClientServicesManager( StateModel *stateModel
                         , ProjectModel *projectModel
                         , iDOMParser *DOMParser );

    ~ClientServicesManager();

    iSessionManagerClientSrv *GetSessionManagerSrv();

protected:
    iSessionManagerClientSrv *m_sessionManagerSrv;
    StateModel *m_stateModel;
    ProjectModel *m_projectModel;
    iDOMParser *m_DOMParser;
};


}   //  namespace ClientServices

#endif  //  #ifndef CLIENTSERVICESMANAGER_H
