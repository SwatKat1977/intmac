//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef SESSIONMANAGERCLIENTSRV_H
#define SESSIONMANAGERCLIENTSRV_H
#include <clientServices\iSessionManagerClientSrv.h>
#include <dataModels\stateModel.h>
#include <dataModels\projectModel.h>
#include <iDOMElement.h>
#include <iDOMParser.h>


namespace ClientServices
{

typedef long ProjectID;

struct  BulkDataInfo
{
    unsigned int m_totalSize;
    unsigned int m_payloadSize;
    char *m_payload;
};


class SessionManagerClientSrv : public iSessionManagerClientSrv
{
public:

    SessionManagerClientSrv( StateModel *stateModel
                           , ProjectModel *projectModel
                           , iDOMParser *DOMParser );

    ~SessionManagerClientSrv();

    bool HandleMessage( MessageProtocol::MessageHeader *header
                      , char *body );

protected:
    bool HandleHandshakeResponse( MessageProtocol::MessageHeader *header, char *body );

    bool HandleAuthenticationStatus( MessageProtocol::MessageHeader *header, char *body );

    bool HandleBeginProjectsImport( MessageProtocol::MessageHeader *header, char *body );

    bool HandleProjectsListBulkData( MessageProtocol::MessageHeader *header, char *body );

    bool HandleSelectProject( MessageProtocol::MessageHeader *header, char *body);

    bool HandleProjectSelectionStatus( MessageProtocol::MessageHeader *header, char *body );

    bool HandleBeginProjectAssets( MessageProtocol::MessageHeader *header, char *body );

    bool HandleProjectAssetsBulkData( MessageProtocol::MessageHeader *header, char *body );

    bool HandleBeginReleasesBulkData( MessageProtocol::MessageHeader *header, char *body );

    bool HandleReleasesBulkData( MessageProtocol::MessageHeader *header, char *body );

    bool ParseProjectsListXML( char *rawXML
                             , ProjectsList &projects
                             , unsigned int sizeOfXML
                             , std::string &errMsg );

    bool ParseProjectAssetsXML( char *rawXML
                              , ProjectAssetsList &assets
                              , unsigned int sizeOfXML
                              , std::string &errMsg );

    bool ParseProjectsListElement( iDOMElement *node
                                 , ProjectEntry &entry );

    bool ParseProjectAssetElement( iDOMElement *node
                                 , ProjectAssetEntry &entry );

    bool ParseReleasesXML( char *rawXML
                         , ReleaseEntriesList &releases
                         , unsigned int sizeOfXML
                         , std::string &errMsg );

    bool ParseReleaseElement( iDOMElement *node
                            , ReleaseEntry &release );

private:
    StateModel *m_stateModel;
    ProjectModel *m_projectModel;
    std::map<MessageProtocol::MessageID, BulkDataInfo> m_toBeProcessedBulkData;
    iDOMParser *m_DOMParser;
};


}   //  namespace ClientServices

#endif  //  #ifndef SESSIONMANAGERCLIENTSRV_H
