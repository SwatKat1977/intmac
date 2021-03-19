//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <DOMParser.h>
#include <XMLParseException.h>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/dom/DOM.hpp>
#include <DOMDocument.h>
#include <ErrorHandler.h>
#include <xercesc/framework/MemBufInputSource.hpp>


DOMParser::DOMParser( ) : m_parser( new xercesc::XercesDOMParser )
{
}


DOMParser::~DOMParser( )
{
    if ( m_parser ) delete m_parser;
}


void DOMParser::setDoNamespaces ( bool newState )
{
    m_parser->setDoNamespaces( newState );  
}


void DOMParser::setValidationScheme ( ValSchemes newScheme )
{
    m_parser->setValidationScheme( ( xercesc::XercesDOMParser::ValSchemes ) newScheme );
}


void DOMParser::parseFile( std::string filename )
{
    try 
    {
        m_parser->parse( filename.c_str() );
    }
    catch ( const xercesc::XMLException &toCatch ) 
    {
        std::string errMsg = "XML Exception Caught : ";
        errMsg.append( xercesc::XMLString::transcode( toCatch.getMessage( ) ) );
        throw XMLParseException( errMsg );
    }
    catch ( const xercesc::DOMException &toCatch )
    {
        std::string errMsg = "DOM Exception Caught : ";
        errMsg.append( xercesc::XMLString::transcode( toCatch.getMessage( ) ) );
        throw XMLParseException( errMsg );
    }
    catch ( const xercesc::SAXParseException &toCatch )
    {
        std::string errMsg = "SAX Exception Caught : ";
        errMsg.append( xercesc::XMLString::transcode( toCatch.getMessage( ) ) );
        throw XMLParseException( errMsg );
    }
    catch( XMLParseException &e )
    {
        throw e;
    }
    catch ( std::exception& e )
    {
        std::string errMsg = "Unexpected Exception Caught : ";
        errMsg.append( e.what( ) );
        throw XMLParseException( errMsg );
    }
}


void DOMParser::parseString( std::string xmlString, size_t stringSize )
{
    try 
    {
        xercesc::MemBufInputSource myxml_buf( ( const XMLByte* ) xmlString.c_str()
                                            , stringSize
                                            , "dummy" );
        m_parser->parse( myxml_buf );
    }
    catch ( const xercesc::XMLException &toCatch ) 
    {
        std::string errMsg = "XML Exception Caught : ";
        errMsg.append( xercesc::XMLString::transcode( toCatch.getMessage( ) ) );
        throw XMLParseException( errMsg );
    }
    catch ( const xercesc::DOMException &toCatch )
    {
        std::string errMsg = "DOM Exception Caught : ";
        errMsg.append( xercesc::XMLString::transcode( toCatch.getMessage( ) ) );
        throw XMLParseException( errMsg );
    }
    catch ( const xercesc::SAXParseException &toCatch )
    {
        std::string errMsg = "SAX Exception Caught : ";
        errMsg.append( xercesc::XMLString::transcode( toCatch.getMessage( ) ) );
        throw XMLParseException( errMsg );
    }
    catch( XMLParseException &e )
    {
        throw e;
    }
    catch ( std::exception& e )
    {
        std::string errMsg = "Unexpected Exception Caught : ";
        errMsg.append( e.what( ) );
        throw XMLParseException( errMsg );
    }
}


iDOMDocument *DOMParser::getDocument( )
{
    return new DOMDocument( m_parser->getDocument( ) ); 
}


bool DOMParser::loadGrammar( std::string schemaName, 
                             GrammarType grammarType, 
                             bool toCache )
{
    bool returnValue = true;

    if ( m_parser->loadGrammar( schemaName.c_str( ),
                                ( xercesc_3_1::Grammar::GrammarType ) grammarType,
                                toCache ) == NULL )
    {
        returnValue = false;
    }

    return returnValue;
}


void DOMParser::setDoSchema( const bool newState )
{
    m_parser->setDoSchema( newState );
}


void DOMParser::setValidationConstraintFatal( const bool newState )
{
    m_parser->setValidationConstraintFatal( newState );
}


const size_t DOMParser::getErrorCount( )
{
    return m_parser->getErrorCount( );
}


void DOMParser::SetErrorHandler( )
{
    ErrorHandler *errHandler = new ErrorHandler;

    m_parser->setErrorHandler( errHandler );
}
