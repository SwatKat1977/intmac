//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <sstream>
#include <clientServices\sessionManagerClientSrv.h>
#include <messageIdentities.h>
#include <clientServerMessages_state.h>
#include <iDOMParser.h>
#include <stringUtils.h>
#include <xml/configNode.h>
#include <XMLParseException.h>

using namespace Common;

namespace ClientServices
{


//  XML tags & attributes for projects list
std::string XMLTag_projects = "PROJECTS";
std::string XMLTag_project = "PROJECT";
std::string XMLAttribute_projectID = "ID";
std::string XMLAttribute_projectName = "NAME";

//  XML tags & attributes for project assets
std::string XMLTag_assets = "ASSETS";
std::string XMLTag_asset = "ASSET";
std::string XMLAttribute_assetID = "ID";
std::string XMLAttribute_assetParentID = "PARENTID";
std::string XMLAttribute_assetName = "NAME";
std::string XMLAttribute_assetType = "TYPE";
std::string XMLAttribute_assetType_filter = "FILTER";
std::string XMLAttribute_assetType_testGroup = "TESTGROUP";
std::string XMLAttribute_assetType_testItem = "TESTITEM";

//  XML tags & attributes for releases
std::string XMLTag_releases = "RELEASES";
std::string XMLTag_release = "RELEASE";
std::string XMLAttribute_releaseID = "ID";
std::string XMLAttribute_releaseParentID = "PARENTID";
std::string XMLAttribute_releaseName = "NAME";
std::string XMLAttribute_releaseType = "TYPE";
std::string XMLAttribute_releaseType_cycle = "CYCLE";
std::string XMLAttribute_releaseType_folder = "FOLDER";
std::string XMLAttribute_releaseType_release = "RELEASE";


SessionManagerClientSrv::SessionManagerClientSrv( StateModel *stateModel
                                                , ProjectModel *projectModel
                                                , iDOMParser *DOMParser )
    : m_stateModel( stateModel ), m_projectModel( projectModel )
    , m_DOMParser( DOMParser )
{
}


SessionManagerClientSrv::~SessionManagerClientSrv()
{
}


bool SessionManagerClientSrv::HandleMessage( MessageProtocol::MessageHeader *header
                                           , char *body )
{
    bool returnStatus = false;

    switch ( header->m_ID )
    {
    case MessageProtocol::MsgID_HandshakeResponse:
        returnStatus = HandleHandshakeResponse( header, body );
        break;

    case MessageProtocol::MsgID_AuthenticationStatus:
        returnStatus = HandleAuthenticationStatus( header, body );
        break;
  
    case MessageProtocol::MsgID_BeginProjectsBulkImport:
        returnStatus = HandleBeginProjectsImport( header, body );
        break;

    case MessageProtocol::MsgID_ProjectsListBulkData:
        returnStatus = HandleProjectsListBulkData( header, body );
        break;

    case MessageProtocol::MsgID_SelectProject:
        returnStatus = HandleSelectProject( header, body );
        break;

    case MessageProtocol::MsgID_ProjectSelectionStatus:
        returnStatus = HandleProjectSelectionStatus( header, body );
        break;

    case MessageProtocol::MsgID_BeginProjectAssetsBulkData:
        returnStatus = HandleBeginProjectAssets( header, body );
        break;

    case MessageProtocol::MsgID_ProjectAssetsBulkData:
        returnStatus = HandleProjectAssetsBulkData( header, body );
        break;

    case MessageProtocol::MsgID_BeginReleasesBulkData:
        returnStatus = HandleBeginReleasesBulkData( header, body );
        break;

    case MessageProtocol::MsgID_ReleasesBulkData:
        returnStatus = HandleReleasesBulkData( header, body );
        break;

    default:
        break;
    }

    return returnStatus;
}


bool SessionManagerClientSrv::HandleHandshakeResponse( MessageProtocol::MessageHeader *header
                                                     , char *body )
{
    //  For time being as there is no authentication system, the handshake is
    //  always marked as successful.
    m_stateModel->SetHandshakeCompleteStatus ( true );

    return true;
}


bool SessionManagerClientSrv::HandleAuthenticationStatus( MessageProtocol::MessageHeader *header
                                                        , char *body )
{
    //  For time being as there is no authentication system, therefore
    //  authentication is always marked as successful.
    m_stateModel->SetRealm( "PreAlphRealm" );
    m_stateModel->SetUser( "SystemAdmin" );

    return true;
}


bool SessionManagerClientSrv::HandleBeginProjectsImport( MessageProtocol::MessageHeader *header
                                                       , char *body )
{
    CSMsg_BeginProjectsBulkImport msgBody;

    memcpy( &msgBody, body, sizeof( CSMsg_BeginProjectsBulkImport ) );

    m_toBeProcessedBulkData[MessageProtocol::MsgID_ProjectsListBulkData].m_payload = new char[msgBody.m_totalPayloadSize];
    m_toBeProcessedBulkData[MessageProtocol::MsgID_ProjectsListBulkData].m_totalSize = msgBody.m_totalPayloadSize;
    memset (m_toBeProcessedBulkData[MessageProtocol::MsgID_ProjectsListBulkData].m_payload
        , 0
        , MaxBulkDataTxPayload);
    m_toBeProcessedBulkData[MessageProtocol::MsgID_ProjectsListBulkData].m_payloadSize = 0;

    return true;
}


bool SessionManagerClientSrv::HandleProjectsListBulkData( MessageProtocol::MessageHeader *header
                                                        , char *body )
{
    MessageProtocol::MessageID msgID = header->m_ID;
    std::string errMsg;

    //  Verify that the bulk data storage block exists, this should never fail,
    //  but is a fail-safe check.
    if ( m_toBeProcessedBulkData.find (msgID) ==
         m_toBeProcessedBulkData.end ())
    {
        return false;
    }

    //  Verify that the bulk data storage block size isn't being exceeded, this
    //  should never occur!
    if ((m_toBeProcessedBulkData[msgID].m_payloadSize + header->m_bodyLength)
         > m_toBeProcessedBulkData[msgID].m_totalSize)
    {
        return false;
    }

    memcpy( m_toBeProcessedBulkData[msgID].m_payload +
            m_toBeProcessedBulkData[msgID].m_payloadSize
          , body
          , header->m_bodyLength );

    m_toBeProcessedBulkData[msgID].m_payloadSize += header->m_bodyLength;

    if (m_toBeProcessedBulkData[msgID].m_payloadSize ==
        m_toBeProcessedBulkData[msgID].m_totalSize)
    {
        ProjectsList projects;

        if ( ParseProjectsListXML( m_toBeProcessedBulkData[msgID].m_payload
                                 , projects
                                 , m_toBeProcessedBulkData[msgID].m_totalSize
                                 , errMsg ) == true )
        {
            m_stateModel->AddProjectsList (projects);
        }

        delete m_toBeProcessedBulkData[msgID].m_payload;

        m_toBeProcessedBulkData.erase( msgID );

        //  This is a temporary - but for now trigger a 'Select Project' 
        HandleSelectProject( NULL, NULL );
    }

    return true;
}


bool SessionManagerClientSrv::HandleSelectProject( MessageProtocol::MessageHeader *header
                                                 , char *body )
{
    bool returnValue = false;

    //  Currently there isn't there any way of selecting a project, therefore
    //  hardcode an initial value...  Currently the first project in the list.
    ProjectsList::iterator it;
    ProjectsList projects = m_stateModel->GetProjectsList().m_projectsList;

    if (projects.size () > 0)
    {
        ProjectEntry entry = *( projects.begin() );

        m_stateModel->SetSelectedProject( entry.m_id, entry.m_name );

        returnValue = true;
    }

    return returnValue;
}


bool SessionManagerClientSrv::HandleProjectSelectionStatus( MessageProtocol::MessageHeader *header
                                                          , char *body )
{

    //  This message isn't currently implemented and will always pass muster!
    return true;
}


bool SessionManagerClientSrv::HandleBeginProjectAssets( MessageProtocol::MessageHeader *header
                                                      , char *body )
{
    CSMsg_BeginProjectAssetsBulkData msgBody;

    memcpy (&msgBody, body, sizeof (CSMsg_BeginProjectAssetsBulkData));

    m_toBeProcessedBulkData[MessageProtocol::MsgID_ProjectAssetsBulkData].m_payload = new char[msgBody.m_totalPayloadSize];
    m_toBeProcessedBulkData[MessageProtocol::MsgID_ProjectAssetsBulkData].m_totalSize = msgBody.m_totalPayloadSize;
    memset (m_toBeProcessedBulkData[MessageProtocol::MsgID_ProjectAssetsBulkData].m_payload
        , 0
        , MaxBulkDataTxPayload);
    m_toBeProcessedBulkData[MessageProtocol::MsgID_ProjectAssetsBulkData].m_payloadSize = 0;

    return true;
}


bool SessionManagerClientSrv::HandleProjectAssetsBulkData( MessageProtocol::MessageHeader *header
                                                         , char *body )
{
    MessageProtocol::MessageID msgID = header->m_ID;
    std::string errMsg;
    bool returnValue = false;

    //  Verify that the bulk data storage block exists, this should never fail,
    //  but is a fail-safe check.
    if (m_toBeProcessedBulkData.find (msgID) ==
        m_toBeProcessedBulkData.end ())
    {
        return false;
    }

    //  Verify that the bulk data storage block size isn't being exceeded, this
    //  should never occur!
    if ((m_toBeProcessedBulkData[msgID].m_payloadSize + header->m_bodyLength)
         > m_toBeProcessedBulkData[msgID].m_totalSize)
    {
        return false;
    }

    memcpy (m_toBeProcessedBulkData[msgID].m_payload +
        m_toBeProcessedBulkData[msgID].m_payloadSize
        , body
        , header->m_bodyLength);

    m_toBeProcessedBulkData[msgID].m_payloadSize += header->m_bodyLength;

    if (m_toBeProcessedBulkData[msgID].m_payloadSize ==
        m_toBeProcessedBulkData[msgID].m_totalSize)
    {
        ProjectAssetsList projectsAssets;

        if ( ParseProjectAssetsXML( m_toBeProcessedBulkData[msgID].m_payload
            , projectsAssets
            , m_toBeProcessedBulkData[msgID].m_totalSize
            , errMsg) == true)
        {
            m_projectModel->AddBulkProjectAssetsData( projectsAssets );
        }

        delete m_toBeProcessedBulkData[msgID].m_payload;

        m_toBeProcessedBulkData.erase (msgID);

        returnValue = true;
    }

    return returnValue;
}


bool SessionManagerClientSrv::HandleBeginReleasesBulkData( MessageProtocol::MessageHeader *header
                                                         , char *body )
{
    CSMsg_BeginReleasesBulkData msgBody;

    memcpy (&msgBody, body, sizeof( CSMsg_BeginReleasesBulkData ) );

    m_toBeProcessedBulkData[MessageProtocol::MsgID_ReleasesBulkData].m_payload = new char[msgBody.m_totalPayloadSize];
    m_toBeProcessedBulkData[MessageProtocol::MsgID_ReleasesBulkData].m_totalSize = msgBody.m_totalPayloadSize;
    memset (m_toBeProcessedBulkData[MessageProtocol::MsgID_ReleasesBulkData].m_payload
        , 0
        , MaxBulkDataTxPayload);
    m_toBeProcessedBulkData[MessageProtocol::MsgID_ReleasesBulkData].m_payloadSize = 0;

    return true;
}


bool SessionManagerClientSrv::HandleReleasesBulkData( MessageProtocol::MessageHeader *header
                                                    , char *body )
{
    MessageProtocol::MessageID msgID = header->m_ID;
    std::string errMsg;

    //  Verify that the bulk data storage block exists, this should never fail,
    //  but is a fail-safe check.
    if (m_toBeProcessedBulkData.find (msgID) ==
        m_toBeProcessedBulkData.end ())
    {
        return false;
    }

    //  Verify that the bulk data storage block size isn't being exceeded, this
    //  should never occur!
    if ((m_toBeProcessedBulkData[msgID].m_payloadSize + header->m_bodyLength)
         > m_toBeProcessedBulkData[msgID].m_totalSize)
    {
        return false;
    }

    memcpy (m_toBeProcessedBulkData[msgID].m_payload +
        m_toBeProcessedBulkData[msgID].m_payloadSize
        , body
        , header->m_bodyLength);

    m_toBeProcessedBulkData[msgID].m_payloadSize += header->m_bodyLength;

    if (m_toBeProcessedBulkData[msgID].m_payloadSize ==
        m_toBeProcessedBulkData[msgID].m_totalSize)
    {
        ReleaseEntriesList releasesAssets;

        if (ParseReleasesXML (m_toBeProcessedBulkData[msgID].m_payload
            , releasesAssets
            , m_toBeProcessedBulkData[msgID].m_totalSize
            , errMsg) == true)
        {
            m_projectModel->AddBulkReleasesAssetsData (releasesAssets);
        }

        delete m_toBeProcessedBulkData[msgID].m_payload;

        m_toBeProcessedBulkData.erase (msgID);

        return true;
    }

    return false;
}


bool SessionManagerClientSrv::ParseProjectsListXML( char *rawXML
                                                  , ProjectsList &projects
                                                  , unsigned int sizeOfXML
                                                  , std::string &errMsg )
{
    bool returnValue = true;
    iDOMElement *node = NULL;
    std::string topLevelKey;
    iDOMDocument *document = NULL;

    try
    {
        m_DOMParser->parseString( rawXML, sizeOfXML );

        document = m_DOMParser->getDocument ();
    }
    catch( XMLParseException &toCatch )
    {
        std::stringstream ss;
        ss << "Parsing of Projects List XML parse failed with error of '"
            << toCatch.what () << "'";
        errMsg = ss.str();
        return false;
    }

    node = document->getDocumentElement();

    topLevelKey = StringUtils::StringToUppercase (node->getNodeName ());

    if (topLevelKey.compare( XMLTag_projects ) == 0)
    {
        iDOMElement *child = NULL;

        for (child = node->getFirstElementChild (); child != 0;
        child = child->getNextElementSibling ())
        {
            std::string childsName = StringUtils::StringToUppercase( child->getNodeName() );

            if (childsName.compare( XMLTag_project ) == 0)
            {
                ProjectEntry entry;

                if (ParseProjectsListElement( child, entry ) == false)
                {
                    errMsg = "Failed to parse an <element> tag!";
                    returnValue = false;
                    break;
                }

                projects.push_back( entry );
            }
            else
            {
                std::stringstream ss;
                ss << "Invalid <element> tag '" << child->getNodeName () << "'";
                errMsg = ss.str();

                returnValue = false;
                break;
            }
        }
    }
    else
    {
        std::stringstream ss;
        ss << "Invalid top-level <elements> tag '" << topLevelKey << "'";
        errMsg = ss.str();

        returnValue = false;
    }

    return returnValue;
}


bool SessionManagerClientSrv::ParseProjectsListElement( iDOMElement *node
                                                      , ProjectEntry &entry )
{
    ConfigNode *childNode = NULL;
    std::string key;
    bool IDAttributeSet = false;
    bool nameAttributeSet = false;
    bool returnStatus = true;

    childNode = new ConfigNode( node );

    for ( size_t i = 0; i < childNode->GetTotalNumberOfAttributes(); ++i )
    {
        ConfigAttribute *attr = NULL;

        attr = childNode->GetAttribute();

        key = StringUtils::StringToUppercase( attr->m_key );

        if (key.compare( XMLAttribute_projectID ) == 0)
        {
            entry.m_id = atol( attr->m_value.c_str() );
            IDAttributeSet = true;
        }
        else if( key.compare ( XMLAttribute_projectName ) == 0 )
        {
            entry.m_name = attr->m_value;
            nameAttributeSet = true;
        }

        childNode->NextAttribute();
    }

    if ( ( IDAttributeSet == false ) || ( nameAttributeSet == false ) )
    {
        returnStatus = false;
    }

    delete childNode;

    return returnStatus;

}


bool SessionManagerClientSrv::ParseProjectAssetsXML( char *rawXML
                                                   , ProjectAssetsList &assets
                                                   , unsigned int sizeOfXML
    , std::string &errMsg)
{
    bool returnValue = true;
    iDOMElement *node = NULL;
    std::string topLevelKey;
    iDOMDocument *document = NULL;

    try
    {
        m_DOMParser->parseString( rawXML, sizeOfXML );

        document = m_DOMParser->getDocument ();
    }
    catch ( XMLParseException &toCatch )
    {
        std::stringstream ss;
        ss << "Parsing of Project Assets XML parse failed with error of '"
            << toCatch.what() << "'";
        errMsg = ss.str();
        return false;
    }

    node = document->getDocumentElement ();

    topLevelKey = StringUtils::StringToUppercase( node->getNodeName() );

    if ( topLevelKey.compare( XMLTag_assets ) == 0 )
    {
        iDOMElement *child = NULL;

        for (child = node->getFirstElementChild (); child != 0;
        child = child->getNextElementSibling ())
        {
            std::string childsName = StringUtils::StringToUppercase (child->getNodeName ());

            if (childsName.compare (XMLTag_asset) == 0)
            {
                ProjectAssetEntry entry;

                if (ParseProjectAssetElement (child, entry) == false)
                {
                    errMsg = "Failed to parse an <asset> tag!";
                    returnValue = false;
                    break;
                }

                assets.push_back (entry);
            }
            else
            {
                std::stringstream ss;
                ss << "Invalid <assets> child tag '" << child->getNodeName () << "'";
                errMsg = ss.str ();

                returnValue = false;
                break;
            }
        }
    }
    else
    {
        std::stringstream ss;
        ss << "Invalid top-level <elements> tag '" << topLevelKey << "'";
        errMsg = ss.str ();

        returnValue = false;
    }

    return returnValue;
}


bool SessionManagerClientSrv::ParseProjectAssetElement( iDOMElement *node
                                                      , ProjectAssetEntry &entry )
{
    ConfigNode *childNode = NULL;
    std::string key;
    bool assetIDAttributeSet = false;
    bool assetParentIDAttributeSet = false;
    bool assetNameAttributeSet = false;
    bool assetTypeAttributeSet = false;
    bool returnStatus = true;

    childNode = new ConfigNode (node);

    for (size_t i = 0; i < childNode->GetTotalNumberOfAttributes (); ++i)
    {
        ConfigAttribute *attr = NULL;

        attr = childNode->GetAttribute ();

        key = StringUtils::StringToUppercase (attr->m_key);

        if (key.compare (XMLAttribute_assetID) == 0)
        {
            entry.m_id = atol (attr->m_value.c_str ());
            assetIDAttributeSet = true;
        }
        else if (key.compare (XMLAttribute_assetParentID) == 0)
        {
            entry.m_parentID = atol (attr->m_value.c_str ());
            assetParentIDAttributeSet = true;
        }
        else if (key.compare (XMLAttribute_assetName) == 0)
        {
            entry.m_name = attr->m_value;
            assetNameAttributeSet = true;
        }
        else if (key.compare (XMLAttribute_assetType) == 0)
        {
            std::string assetTypeKeyStr;

            assetTypeKeyStr = StringUtils::StringToUppercase (attr->m_value);

            if (assetTypeKeyStr.compare (XMLAttribute_assetType_filter) == 0)
            {
                entry.m_type = AssetItemType_filter;
                assetTypeAttributeSet = true;
            }
            else if (assetTypeKeyStr.compare (XMLAttribute_assetType_testGroup) == 0)
            {
                entry.m_type = AssetItemType_testGroup;
                assetTypeAttributeSet = true;
            }
            else if (assetTypeKeyStr.compare (XMLAttribute_assetType_testItem) == 0)
            {
                entry.m_type = AssetItemType_testItem;
                assetTypeAttributeSet = true;
            }
            else
            {
                assetTypeAttributeSet = false;
            }
        }

        childNode->NextAttribute ();
    }

    if ((assetIDAttributeSet == false) ||
        (assetParentIDAttributeSet == false) ||
        (assetNameAttributeSet == false) ||
        (assetTypeAttributeSet == false))
    {
        returnStatus = false;
    }

    delete childNode;

    return returnStatus;
}


bool SessionManagerClientSrv::ParseReleasesXML( char *rawXML
                                              , ReleaseEntriesList &releases
                                              , unsigned int sizeOfXML
                                              , std::string &errMsg)
{
    bool returnValue = true;
    iDOMElement *node = NULL;
    std::string topLevelKey;
    iDOMDocument *document = NULL;

    try
    {
        m_DOMParser->parseString (rawXML, sizeOfXML);

        document = m_DOMParser->getDocument ();
    }
    catch (XMLParseException &toCatch)
    {
        std::stringstream ss;
        ss << "Parsing of Releases XML parse failed with error of '"
            << toCatch.what () << "'";
        errMsg = ss.str ();
        return false;
    }

    node = document->getDocumentElement ();

    topLevelKey = StringUtils::StringToUppercase (node->getNodeName ());

    if (topLevelKey.compare (XMLTag_releases) == 0)
    {
        iDOMElement *child = NULL;

        for (child = node->getFirstElementChild (); child != 0;
        child = child->getNextElementSibling ())
        {
            std::string childsName = StringUtils::StringToUppercase (child->getNodeName ());

            if (childsName.compare (XMLTag_release) == 0)
            {
                ReleaseEntry entry;

                if (ParseReleaseElement (child, entry) == false)
                {
                    errMsg = "Failed to parse an <element> tag!";
                    returnValue = false;
                    break;
                }

                releases.push_back (entry);
            }
            else
            {
                std::stringstream ss;
                ss << "Invalid <element> tag '" << child->getNodeName () << "'";
                errMsg = ss.str ();

                returnValue = false;
                break;
            }
        }
    }
    else
    {
        std::stringstream ss;
        ss << "Invalid top-level <elements> tag '" << topLevelKey << "'";
        errMsg = ss.str ();

        returnValue = false;
    }

    return returnValue;
}


bool SessionManagerClientSrv::ParseReleaseElement( iDOMElement *node
                                                 , ReleaseEntry &entry )
{
    ConfigNode *childNode = NULL;
    std::string key;
    bool attributeSet_releaseID = false;
    bool attributeSet_parentID = false;
    bool attributeSet_name = false;
    bool attributeSet_type = false;
    bool returnStatus = true;

    childNode = new ConfigNode (node);

    for (size_t i = 0; i < childNode->GetTotalNumberOfAttributes (); ++i)
    {
        ConfigAttribute *attr = NULL;

        attr = childNode->GetAttribute ();

        key = StringUtils::StringToUppercase (attr->m_key);

        if (key.compare (XMLAttribute_releaseID) == 0)
        {
            entry.m_id = atol (attr->m_value.c_str ());
            attributeSet_releaseID = true;
        }
        else if (key.compare (XMLAttribute_releaseParentID) == 0)
        {
            entry.m_parentID = atol (attr->m_value.c_str ());
            attributeSet_parentID = true;
        }
        else if (key.compare (XMLAttribute_releaseName) == 0)
        {
            entry.m_name = attr->m_value;
            attributeSet_name = true;
        }
        else if (key.compare (XMLAttribute_releaseType) == 0)
        {
            std::string assetTypeKeyStr;

            assetTypeKeyStr = StringUtils::StringToUppercase (attr->m_value);

            if (assetTypeKeyStr.compare (XMLAttribute_releaseType_folder) == 0)
            {
                entry.m_type = ReleaseItemType_folder;
                attributeSet_type = true;
            }
            else if (assetTypeKeyStr.compare (XMLAttribute_releaseType_cycle) == 0)
            {
                entry.m_type = ReleaseItemType_cycle;
                attributeSet_type = true;
            }
            else if (assetTypeKeyStr.compare (XMLAttribute_releaseType_release) == 0)
            {
                entry.m_type = ReleaseItemType_release;
                attributeSet_type = true;
            }
            else
            {
                attributeSet_type = false;
            }
        }

        childNode->NextAttribute ();
    }

    if ((attributeSet_releaseID == false) ||
        (attributeSet_parentID == false) ||
        (attributeSet_name == false) ||
        (attributeSet_type == false))
    {
        returnStatus = false;
    }

    delete childNode;

    return returnStatus;
}


}   //  namespace ClientServices
