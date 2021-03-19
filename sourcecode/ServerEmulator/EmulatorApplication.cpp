#include <iostream>
#include <algorithm>
#include <SharedMemoryFactory.h>
#include <iSharedMemory.h>
#include <iSharedMemoryBlock.h>
#include <sharedMemoryAdaptor.h>
#include <IntMacException.h>
#include <sharedMemoryInfo.h>
#include <MsgProtocolDefs.h>
#include <clientServerMessages_state.h>
#include <iDatabaseAdaptor.h>
#include <SQLiteAdaptor.h>
#include <MessageIdentities.h>


using namespace Common;


struct shMemControlBlock
{
    unsigned int m_bufferSize;
};

std::string shMemReadBufferName = "Global\\intMacReadBuffer";
std::string shMemWriteBufferName = "Global\\intMacWriteBuffer";
const int shMemReadWriteBufferSize = 1024;
DatabaseAdaptor::iDatabaseObject  *dbObject = NULL;
DatabaseAdaptor::SQLiteAdaptor *adptor = NULL;

char szReadMsg[]  = "[INFO] Msg from read process.";
char szWriteMsg[]  = "[INFO] Msg from write process.";

/*
std::string releasesListXML = 
"<releases> \
     <release id=\"1\" parentID=\"0\" name=\"Server-Side\" type=\"folder\"/> \
     <release id=\"2\" parentID=\"0\" name=\"Client-Side\" type=\"folder\"/> \
     <release id=\"3\" parentID=\"2\" name=\"Client\" type=\"folder\"/> \
     <release id=\"4\" parentID=\"3\" name=\"V0.1.0\" type=\"release\"/> \
     <release id=\"5\" parentID=\"3\" name=\"V0.2.0\" type=\"release\"/> \
     <release id=\"6\" parentID=\"5\" name=\"Cycle 1\" type=\"cycle\"/> \
     <release id=\"7\" parentID=\"5\" name=\"Cycle 2\" type=\"cycle\"/> \
     <release id=\"8\" parentID=\"1\" name=\"Server\" type=\"release\"/> \
     <release id=\"9\" parentID=\"1\" name=\"Server Emulator\" type=\"release\"/> \
</releases>";
const int releasesListXMLSize = releasesListXML.size();
*/

std::string GenerateProjectsList()
{
    std::string projectXML;
    std::string queryStr = "SELECT * FROM project";

    if ( dbObject->RawQuery( queryStr ) == false )
    {
        throw std::exception( "Querying of 'project' table failed!" );
    }

    DatabaseAdaptor::iResultsSet *results = dbObject->GetResults();

    //  If no results then just return an empty string
    if ( results->Size() == 0 )
    {
        return "";
    }

    projectXML = "<projects>";

    //  Loop through all of the rows, extracting the data
    for ( int i = 0; i < results->Size(); i++ )
    {
        std::string projectID;
        std::string projectName;

        DatabaseAdaptor::iResultsRow *row = results->GetRow( i );

        for ( int n = 0; n < row->Size(); n++ )
        {
            std::string key = row->GetColumn( n )->GetKey();
            std::transform( key.begin(), key.end(), key.begin(), ::toupper );

            if ( key.compare( "ID" ) == 0 )
            {
                projectID = row->GetColumn( n )->GetValue(); 
            }
            else if ( key.compare( "NAME" ) == 0 )
            {
                projectName = row->GetColumn( n )->GetValue(); 
            }
        }

        if ( !projectID.size() || !projectName.size() )
        {
            throw std::exception( "Invalid query for 'project' failed!" );
        }

        std::string line = "<project id=\"" +
                           projectID +
                           "\" name=\"" +
                           projectName +
                           "\"/>";

        projectXML = projectXML + line;
    }

    projectXML = projectXML + "</projects>";

    return projectXML;
}


std::string GenerateAssetsList( int projectID )
{
    std::string assetsXML;
    std::string queryStr;

    queryStr = "SELECT asset.*, assetParent.parentID as parentID FROM asset \
                INNER JOIN \
                  (SELECT assetID \
                  FROM projectAssetMap \
                  WHERE projectID=" + std::to_string( projectID ) + ") \
                  ON asset.ID = assetID \
                INNER JOIN assetparent ON asset.ID=assetparent.childID";

    if ( dbObject->RawQuery( queryStr ) == false )
    {
        throw std::exception( "Unable to generate assets list for project!" );
    }

    DatabaseAdaptor::iResultsSet *results = dbObject->GetResults();

    //  If no results then just return an empty string
    if ( results->Size() == 0 )
    {
        return "";
    }

    assetsXML = "<assets>";

    //  Loop through all of the rows, extracting the data
    for ( int i = 0; i < results->Size(); i++ )
    {
        std::string assetID;
        std::string assetName;
        std::string assetType;
        std::string assetParentID;

        DatabaseAdaptor::iResultsRow *row = results->GetRow( i );

        for ( int n = 0; n < row->Size(); n++ )
        {
            std::string key = row->GetColumn( n )->GetKey();
            std::transform( key.begin(), key.end(), key.begin(), ::toupper );

            if ( key.compare( "ID" ) == 0 )
            {
                assetID = row->GetColumn( n )->GetValue(); 
            }
            else if ( key.compare( "NAME" ) == 0 )
            {
                assetName = row->GetColumn( n )->GetValue(); 
            }
            else if ( key.compare( "TYPE" ) == 0 )
            {
                if ( row->GetColumn( n )->GetValue().compare( "F" ) == 0 )
                {
                    assetType = "filter"; 
                }
                else if ( row->GetColumn( n )->GetValue().compare( "I" ) == 0 )
                {
                    assetType = "testItem"; 
                }
                else if ( row->GetColumn( n )->GetValue().compare( "G" ) == 0 )
                {
                    assetType = "testGroup"; 
                }
            }
            else if ( key.compare( "PARENTID" ) == 0 )
            {
                assetParentID = row->GetColumn( n )->GetValue(); 
            }
        }

        if ( !assetID.size() || !assetName.size() || 
             !assetType.size() || !assetParentID.size() )
        {
            throw std::exception( "Invalid query for project assets failed!" );
        }

        std::string line = "<asset id=\"" + assetID + "\" " +
                           "parentID=\"" + assetParentID + "\" " +
                           "name=\"" + assetName + "\" " +
                           "type=\"" + assetType + "\" " +
                           "/>";

        assetsXML = assetsXML + line;
    }

    assetsXML = assetsXML + "</assets>";

    return assetsXML;
}


bool IsSharedMemoryCleared( Shared_Memory::iSharedMemoryBlock *shMemBlock )
{
    shMemControlBlock shMemCtrlBlock;

    shMemBlock->Read( sizeof( shMemControlBlock )
                    , 0
                    , ( char * ) &shMemCtrlBlock );

    return ( shMemCtrlBlock.m_bufferSize == 0 ) ? true : false;
}


bool SendProjectsBulkData( shMemControlBlock shMemControlBlock
                         , Shared_Memory::iSharedMemoryBlock *readBlock
                         , Shared_Memory::iSharedMemoryBlock *writeBlock
                         , std::string projectsListXML )
{
    MessageProtocol::MessageHeader messageHeader;
    int bufferToWrite = 0;
    bufferToWrite = projectsListXML.size();

    while ( bufferToWrite > 0 )
    {
        char rawSharedMemoryMessage[1024];
        bool sharedMemoryCleared = false;
        char bulkData[MessageProtocol::MaxMessageBodySize];
        int msgBodySize = ( bufferToWrite >= MessageProtocol::MaxMessageBodySize )
                          ? MessageProtocol::MaxMessageBodySize
                          : bufferToWrite;

        shMemControlBlock.m_bufferSize = MessageProtocol::MessageHeaderSize
                                       + msgBodySize;
    
        messageHeader.m_bodyLength = msgBodySize;
        messageHeader.m_ID = MessageProtocol::MsgID_ProjectsListBulkData;

        memcpy( bulkData
              , projectsListXML.c_str() + ( projectsListXML.size() - bufferToWrite )
              , msgBodySize );

        memcpy( rawSharedMemoryMessage
              , &shMemControlBlock
              , sizeof( shMemControlBlock ) );

        memcpy( rawSharedMemoryMessage + sizeof( shMemControlBlock )
              , &messageHeader
              , MessageProtocol::MessageHeaderSize );

        memcpy( rawSharedMemoryMessage + 
                ( sizeof( shMemControlBlock ) +
                  MessageProtocol::MessageHeaderSize )
              , &bulkData
              , msgBodySize );

        readBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );
        writeBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );

        while( sharedMemoryCleared == false )
        {
            Sleep( 10 );
            sharedMemoryCleared = IsSharedMemoryCleared( readBlock );
        }

        std::cout << "[DEBUG] ProjectsBulk Data Msg of " 
                  << std::to_string( msgBodySize ) 
                  << "bytes [Processed]" << std::endl;
        sharedMemoryCleared = false;

        bufferToWrite -= msgBodySize;
    }

    return true;
}


bool SendAssetsBulkData( shMemControlBlock shMemControlBlock
                       , Shared_Memory::iSharedMemoryBlock *readBlock
                       , Shared_Memory::iSharedMemoryBlock *writeBlock
                       , std::string assetsListXML )
{
    MessageProtocol::MessageHeader messageHeader;
    int bufferToWrite = 0;
    bufferToWrite = assetsListXML.size();

    while ( bufferToWrite > 0 )
    {
        char rawSharedMemoryMessage[1024];
        bool sharedMemoryCleared = false;
        char bulkData[MessageProtocol::MaxMessageBodySize];
        int msgBodySize = ( bufferToWrite >= MessageProtocol::MaxMessageBodySize )
                          ? MessageProtocol::MaxMessageBodySize 
                          : bufferToWrite;

        shMemControlBlock.m_bufferSize = MessageProtocol::MessageHeaderSize 
                                       + msgBodySize;
    
        messageHeader.m_bodyLength = msgBodySize;
        messageHeader.m_ID = MessageProtocol::MsgID_ProjectAssetsBulkData;

        memcpy( bulkData
              , assetsListXML.c_str() + ( assetsListXML.size() - bufferToWrite )
              , msgBodySize );

        memcpy( rawSharedMemoryMessage
              , &shMemControlBlock
              , sizeof( shMemControlBlock ) );

        memcpy( rawSharedMemoryMessage + sizeof( shMemControlBlock )
              , &messageHeader
              , MessageProtocol::MessageHeaderSize );

        memcpy( rawSharedMemoryMessage + 
                ( sizeof( shMemControlBlock ) +
                  MessageProtocol::MessageHeaderSize )
              , &bulkData
              , msgBodySize );

        readBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );
        writeBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );

        while( sharedMemoryCleared == false )
        {
            Sleep( 10 );
            sharedMemoryCleared = IsSharedMemoryCleared( readBlock );
        }

        std::cout << "[DEBUG] AssetsBulk Data Msg of " 
                  << std::to_string( msgBodySize ) 
                  << "bytes [Processed]" << std::endl;
        sharedMemoryCleared = false;

        bufferToWrite -= msgBodySize;
    }

    return true;
}


int SrvEmulator( Common::Shared_Memory::iSharedMemory *shMemoryRoot )
{
    Shared_Memory::iSharedMemoryBlock *readBlock = NULL;
    Shared_Memory::iSharedMemoryBlock *writeBlock = NULL;
    shMemControlBlock shMemControlBlock;
    MessageProtocol::MessageHeader messageHeader;
    char rawSharedMemoryMessage[1024];
    bool sharedMemoryCleared = false;

    //  Create a database adaptor and then connect to the database.
    adptor = new DatabaseAdaptor::SQLiteAdaptor;
    dbObject = adptor->Connect( "emulator.db", SQLITE_OPEN_READWRITE );

    //  Check if database was successfully opened...
    if ( dbObject->IsValid() == false )
    {
        std::cout << "[FATAL ERROR] Unable to open SQL database!" << std::endl;
        return 0;
    }

    try
    {
        readBlock = shMemoryRoot->CreateSharedMemory( shMemReadBufferName
                                                    , shMemReadWriteBufferSize );
        writeBlock = shMemoryRoot->CreateSharedMemory( shMemWriteBufferName
                                                     , shMemReadWriteBufferSize );
    }
    catch( IntMacException &except )
    {
        std::cout << "[FATAL ERROR] " << except.what() << std::endl;
        return 0;
    }


    //  =========================================
    //  ==  1st message => Handshake Response  ==
    //  =========================================
    shMemControlBlock.m_bufferSize = MessageProtocol::MessageHeaderSize;
    
    messageHeader.m_bodyLength = 0;
    messageHeader.m_ID = MessageProtocol::MsgID_HandshakeResponse;

    memcpy( rawSharedMemoryMessage
          , &shMemControlBlock
          , sizeof( shMemControlBlock ) );

    memcpy( rawSharedMemoryMessage + sizeof( shMemControlBlock )
          , &messageHeader
          , MessageProtocol::MessageHeaderSize );

    sharedMemoryCleared = false;
    readBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );
    writeBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );

    while( sharedMemoryCleared == false )
    {
        Sleep( 10 );
        sharedMemoryCleared = IsSharedMemoryCleared( readBlock );
    }

    std::cout << "[DEBUG] 1st Message => Handshake Response [Processed]" << std::endl;
    sharedMemoryCleared = false;


    //  ============================================
    //  ==  2nd message => Authentication Status  ==
    //  ============================================
    CSMsg_AuthenticationStatus msgBody02;

    shMemControlBlock.m_bufferSize = MessageProtocol::MessageHeaderSize 
                                   + sizeof( CSMsg_AuthenticationStatus );
    
    messageHeader.m_bodyLength = sizeof( CSMsg_AuthenticationStatus );
    messageHeader.m_ID = MessageProtocol::MsgID_AuthenticationStatus;

    msgBody02.m_Status = 0x0;

    memcpy( rawSharedMemoryMessage
          , &shMemControlBlock
          , sizeof( shMemControlBlock ) );

    memcpy( rawSharedMemoryMessage + sizeof( shMemControlBlock )
          , &messageHeader
          , MessageProtocol::MessageHeaderSize );

    memcpy( rawSharedMemoryMessage + 
            ( sizeof( shMemControlBlock ) +
              MessageProtocol::MessageHeaderSize )
          , &msgBody02
          , sizeof( CSMsg_AuthenticationStatus ) );

    readBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );
    writeBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );

    while( sharedMemoryCleared == false )
    {
        Sleep( 10 );
        sharedMemoryCleared = IsSharedMemoryCleared( readBlock );
    }

    std::cout << "[DEBUG] 2nd Message => Authentication Status [Processed]"
              << std::endl;
    sharedMemoryCleared = false;


    //  =================================================
    //  ==  3rd message => Begin Projects Bulk Import  ==
    //  =================================================
    std::string projectsListXML;
    CSMsg_BeginProjectsBulkImport msgBody03;

    try
    {
        projectsListXML = GenerateProjectsList();
    }
    catch( std::exception &e )
    {
        std::cout << "[FATAL ERROR] " << e.what() << std::endl;
        return false;
    }

    shMemControlBlock.m_bufferSize = MessageProtocol::MessageHeaderSize 
                                   + sizeof( CSMsg_BeginProjectsBulkImport );
    
    messageHeader.m_bodyLength = sizeof( CSMsg_BeginProjectsBulkImport );
    messageHeader.m_ID = MessageProtocol::MsgID_BeginProjectsBulkImport;

    msgBody03.m_totalPayloadSize = projectsListXML.size();

    memcpy( rawSharedMemoryMessage
          , &shMemControlBlock
          , sizeof( shMemControlBlock ) );

    memcpy( rawSharedMemoryMessage + sizeof( shMemControlBlock )
          , &messageHeader
          , MessageProtocol::MessageHeaderSize );

    memcpy( rawSharedMemoryMessage + 
            ( sizeof( shMemControlBlock ) +
              MessageProtocol::MessageHeaderSize )
          , &msgBody03
          , sizeof( CSMsg_AuthenticationStatus ) );

    readBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );
    writeBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );

    while( sharedMemoryCleared == false )
    {
        Sleep( 10 );
        sharedMemoryCleared = IsSharedMemoryCleared( readBlock );
    }

    std::cout << "[DEBUG] 3rd Message => Begin Projects Bulk Import [Processed]"
              << std::endl;
    sharedMemoryCleared = false;


    //  ==============================================
    //  ==  4th message => Send Projects Bulk Data  ==
    //  ==============================================
    if ( !SendProjectsBulkData( shMemControlBlock
                              , readBlock
                              , writeBlock
                              , projectsListXML ) )
    {
        return 0;
    }


    //  ===============================================
    //  ==  5th message => Project Selection Status  ==
    //  ===============================================
    {
    CSMsg_ProjectSelectionStatus msgBodyProjectSelectionStatus;

    shMemControlBlock.m_bufferSize = MessageProtocol::MessageHeaderSize 
                                   + sizeof( CSMsg_ProjectSelectionStatus );
    
    messageHeader.m_bodyLength = sizeof( CSMsg_ProjectSelectionStatus );
    messageHeader.m_ID = MessageProtocol::MsgID_ProjectSelectionStatus;

    msgBodyProjectSelectionStatus.m_Status = 0x0;

    memcpy( rawSharedMemoryMessage
          , &shMemControlBlock
          , sizeof( shMemControlBlock ) );

    memcpy( rawSharedMemoryMessage + sizeof( shMemControlBlock )
          , &messageHeader
          , MessageProtocol::MessageHeaderSize );

    memcpy( rawSharedMemoryMessage + 
            ( sizeof( shMemControlBlock ) +
              MessageProtocol::MessageHeaderSize )
          , &msgBodyProjectSelectionStatus
          , sizeof( CSMsg_ProjectSelectionStatus ) );

    readBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );
    writeBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );

    while( sharedMemoryCleared == false )
    {
        Sleep( 10 );
        sharedMemoryCleared = IsSharedMemoryCleared( readBlock );
    }

    std::cout << "[DEBUG] 7th Message => Project Selection Status [Processed]"
              << std::endl;
    sharedMemoryCleared = false;
    }


    //  =====================================================
    //  ==  8th message => Begin Project Assets Bulk Data  ==
    //  =====================================================
    CSMsg_BeginProjectAssetsBulkData msgBodyBeginProjectAssetsBulkData;
    std::string assetsListXML;

    try
    {
        assetsListXML = GenerateAssetsList( 1 );
    }
    catch( std::exception &e )
    {
        std::cout << "[FATAL ERROR] " << e.what() << std::endl;
        return false;
    }

    shMemControlBlock.m_bufferSize = MessageProtocol::MessageHeaderSize 
                                   + sizeof( CSMsg_BeginProjectAssetsBulkData );
    
    messageHeader.m_bodyLength = sizeof( CSMsg_BeginProjectAssetsBulkData );
    messageHeader.m_ID = MessageProtocol::MsgID_BeginProjectAssetsBulkData;

    msgBodyBeginProjectAssetsBulkData.m_totalPayloadSize = assetsListXML.size();

    memcpy( rawSharedMemoryMessage
          , &shMemControlBlock
          , sizeof( shMemControlBlock ) );

    memcpy( rawSharedMemoryMessage + sizeof( shMemControlBlock )
          , &messageHeader
          , MessageProtocol::MessageHeaderSize );

    memcpy( rawSharedMemoryMessage + 
            ( sizeof( shMemControlBlock ) +
              MessageProtocol::MessageHeaderSize )
          , &msgBodyBeginProjectAssetsBulkData
          , sizeof( CSMsg_BeginProjectAssetsBulkData ) );

    readBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );
    writeBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );

    while( sharedMemoryCleared == false )
    {
        Sleep( 10 );
        sharedMemoryCleared = IsSharedMemoryCleared( readBlock );
    }

    std::cout << "[DEBUG] 8th Message => Begin Project Assets Bulk Data [Processed]"
              << std::endl;
    sharedMemoryCleared = false;


    //  ====================================================
    //  ==  9th message => Send Project Assets Bulk Data  ==
    //  ====================================================
    if ( !SendAssetsBulkData( shMemControlBlock
                            , readBlock
                            , writeBlock
                            , assetsListXML ) )
    {
        return 0;
    }

#ifdef DONTUSETHIS

    //  ======================================================
    //  ==  11th message => Begin Release Assets Bulk Data  ==
    //  ======================================================
    {
    CSMsg_BeginReleasesBulkData msgBody;

    shMemControlBlock.m_bufferSize = MessageProtocol::MessageHeaderSize 
                                   + sizeof( CSMsg_BeginReleasesBulkData );
    
    messageHeader.m_bodyLength = sizeof( CSMsg_BeginReleasesBulkData );
    messageHeader.m_msgID = MsgID_BeginReleasesBulkData;
    messageHeader.m_msgType = MsgType_State;

    msgBody.m_totalPayloadSize = releasesListXMLSize;

    memcpy( rawSharedMemoryMessage
          , &shMemControlBlock
          , sizeof( shMemControlBlock ) );

    memcpy( rawSharedMemoryMessage + sizeof( shMemControlBlock )
          , &messageHeader
          , MessageProtocol::MessageHeaderSize );

    memcpy( rawSharedMemoryMessage + 
            ( sizeof( shMemControlBlock ) +
              MessageProtocol::MessageHeaderSize )
          , &msgBody
          , sizeof( CSMsg_BeginReleasesBulkData ) );

    readBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );
    writeBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );

    while( sharedMemoryCleared == false )
    {
        Sleep( 10 );
        sharedMemoryCleared = IsSharedMemoryCleared( readBlock );
    }

    std::cout << "[DEBUG] 11th Message => Begin Release Assets Bulk Data [Processed]"
              << std::endl;
    sharedMemoryCleared = false;
    }
    std::cout << "[DEBUG] Sending 619 bytes of Releases XML Data..."
              << std::endl;


    //  ======================================================
    //  ==  12th message => Part 1/2 of Releases Bulk Data  ==
    //  ======================================================
    {
    char msgBody[MessageProtocol::MaxMessageBodySize];

    shMemControlBlock.m_bufferSize = MessageProtocol::MessageHeaderSize 
                                   + MessageProtocol::MaxMessageBodySize;
    
    messageHeader.m_bodyLength = MessageProtocol::MaxMessageBodySize;
    messageHeader.m_msgID = MsgID_ReleasesBulkData;
    messageHeader.m_msgType = MsgType_State;

    memcpy( msgBody
          , releasesListXML.c_str()
          , MessageProtocol::MaxMessageBodySize );

    memcpy( rawSharedMemoryMessage
          , &shMemControlBlock
          , sizeof( shMemControlBlock ) );

    memcpy( rawSharedMemoryMessage + sizeof( shMemControlBlock )
          , &messageHeader
          , MessageProtocol::MessageHeaderSize );

    memcpy( rawSharedMemoryMessage + 
            ( sizeof( shMemControlBlock ) +
              MessageProtocol::MessageHeaderSize )
          , &msgBody
          , MessageProtocol::MaxMessageBodySize );

    readBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );
    writeBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );

    while( sharedMemoryCleared == false )
    {
        Sleep( 10 );
        sharedMemoryCleared = IsSharedMemoryCleared( readBlock );
    }

    std::cout << "[DEBUG] 12th Message => 1st Releases Bulk Data [Processed]"
              << std::endl;
    sharedMemoryCleared = false;
    }


    //  ======================================================
    //  ==  13th message => Part 2/2 of Releases Bulk Data  ==
    //  ======================================================
    {
    char msgBody[MessageProtocol::MaxMessageBodySize];
    int bufferAlreadyRead = MessageProtocol::MaxMessageBodySize;
    int bufferLeft = releasesListXMLSize - bufferAlreadyRead;

    shMemControlBlock.m_bufferSize = MessageProtocol::MessageHeaderSize 
                                   + bufferLeft;
    
    messageHeader.m_bodyLength = bufferLeft;
    messageHeader.m_msgID = MsgID_ReleasesBulkData;
    messageHeader.m_msgType = MsgType_State;

    memcpy( msgBody
          , releasesListXML.c_str() + bufferAlreadyRead
          , bufferLeft );

    memcpy( rawSharedMemoryMessage
          , &shMemControlBlock
          , sizeof( shMemControlBlock ) );

    memcpy( rawSharedMemoryMessage + sizeof( shMemControlBlock )
          , &messageHeader
          , MessageProtocol::MessageHeaderSize );

    memcpy( rawSharedMemoryMessage + 
            ( sizeof( shMemControlBlock ) +
              MessageProtocol::MessageHeaderSize )
          , &msgBody
          , bufferLeft );

    readBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );
    writeBlock->Write( rawSharedMemoryMessage, sizeof( rawSharedMemoryMessage ), 0 );

    while( sharedMemoryCleared == false )
    {
        Sleep( 10 );
        sharedMemoryCleared = IsSharedMemoryCleared( readBlock );
    }

    std::cout << "[DEBUG] 13th Message => 2nd Releases Bulk Data [Processed]"
              << std::endl;
    sharedMemoryCleared = false;
    }

#endif  //  #ifdef DONTUSETHIS


///////////////////////
///////////////////////

    readBlock->Destroy();
    writeBlock->Destroy();

    return 0;
}


int Client( Common::Shared_Memory::iSharedMemory *shMemoryRoot )
{
    char readBuffer[shMemReadWriteBufferSize] = "\0";
    char writeBuffer[shMemReadWriteBufferSize] = "\0";
    Shared_Memory::iSharedMemoryBlock *readBlock = NULL;
    Shared_Memory::iSharedMemoryBlock *writeBlock = NULL;

    try
    {
        readBlock = shMemoryRoot->ConnectToSharedMemory( shMemReadBufferName
                                                       , shMemReadWriteBufferSize );
        writeBlock = shMemoryRoot->ConnectToSharedMemory( shMemWriteBufferName
                                                        , shMemReadWriteBufferSize );
    }
    catch( IntMacException &except )
    {
        std::cout << "[FATAL ERROR] " << except.what() << std::endl;
        return 0;
    }

    readBlock->Read( sizeof( szReadMsg ), 0, readBuffer );
    writeBlock->Read( sizeof( szWriteMsg ), 0, writeBuffer );

    MessageBox( NULL
              , readBuffer, TEXT("Process2::readBlock"), MB_OK);
    MessageBox( NULL
              , writeBuffer, TEXT("Process2::writeBlock"), MB_OK);

    readBlock->Destroy();
    writeBlock->Destroy();

    return 0;
}


int main( int argc, char **argv )
{
    Shared_Memory::iSharedMemory *shMemory = NULL;
    Shared_Memory::SharedMemoryFactory *shmemFactory = NULL;
    Shared_Memory::SharedMemoryAdaptor *shMemAdaptor = NULL;

    shMemAdaptor = new Shared_Memory::SharedMemoryAdaptor;
    shmemFactory = new Shared_Memory::SharedMemoryFactory( shMemAdaptor );

    shMemory = shmemFactory->Create();

    std::cout << "[INFO] INTMAC Message Emulator" << std::endl;
    std::cout << "[INFO] Using Shared Memory Library v" 
              << Shared_Memory::sharedMemoryLibraryVer << std::endl;

    if ( argc == 1 )
    {
        std::cout << "[INFO] Server Emulator" << std::endl;
        return SrvEmulator( shMemory );
    }

    std::cout << "[INFO] Client" << std::endl;
    return Client( shMemory );
}
