//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef ICLIENTSERVICESMANAGER_H
#define ICLIENTSERVICESMANAGER_H
#include <clientServices\iSessionManagerClientSrv.h>


namespace ClientServices
{


class iClientServicesManager
{
public:
    virtual ~iClientServicesManager() {}

    virtual iSessionManagerClientSrv *GetSessionManagerSrv() = 0;
};


}   //  namespace ClientServices

#endif  //  #ifndef ICLIENTSERVICESMANAGER_H
