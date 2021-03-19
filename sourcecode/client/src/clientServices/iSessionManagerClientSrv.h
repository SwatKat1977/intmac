//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef ISESSIONMANAGERCLIENTSRV_H
#define ISESSIONMANAGERCLIENTSRV_H
#include <clientServices\iClientService.h>


namespace ClientServices
{

class iSessionManagerClientSrv : public iClientService
{
public:
    virtual ~iSessionManagerClientSrv() {}

protected:
    virtual bool HandleHandshakeResponse( MessageProtocol::MessageHeader *header
                                        , char *body ) = 0;

    virtual bool HandleAuthenticationStatus( MessageProtocol::MessageHeader *header
                                           , char *body ) = 0;

    virtual bool HandleBeginProjectsImport( MessageProtocol::MessageHeader *header
                                          , char *body ) = 0;

    virtual bool HandleProjectsListBulkData( MessageProtocol::MessageHeader *header
                                           , char *body ) = 0;

    virtual bool HandleSelectProject( MessageProtocol::MessageHeader *header
                                    , char *body ) = 0;

    virtual bool HandleProjectSelectionStatus( MessageProtocol::MessageHeader *header
                                             , char *body ) = 0;

    virtual bool HandleBeginProjectAssets( MessageProtocol::MessageHeader *header
                                         , char *body ) = 0;

    virtual bool HandleProjectAssetsBulkData( MessageProtocol::MessageHeader *header
                                            , char *body ) = 0;

    virtual bool HandleBeginReleasesBulkData( MessageProtocol::MessageHeader *header
                                            , char *body ) = 0;

    virtual bool HandleReleasesBulkData( MessageProtocol::MessageHeader *header
                                       , char *body ) = 0;
};


}   //  namespace ClientServices

#endif  //  #ifndef ISESSIONMANAGERCLIENTSRV_H
