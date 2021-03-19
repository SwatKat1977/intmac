//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef DOMPARSER_H
#define DOMPARSER_H

#include <iDOMParser.h>
#include <xercesc/parsers/XercesDOMParser.hpp>


class DOMParser : public iDOMParser
{
public:
    DOMParser( );

    ~DOMParser( );

    void setDoNamespaces ( bool newState );

    void setDoSchema( bool newState );

    void SetErrorHandler( );

    void setValidationConstraintFatal( bool newState );

    void setValidationScheme ( ValSchemes newScheme );

    const size_t getErrorCount ();

    void parseFile( std::string filename );

    void parseString( std::string xmlString, size_t stringSize );

    iDOMDocument *getDocument( );

    bool loadGrammar( std::string schemaName, 
                      GrammarType grammarType,
                      bool toCache = false );

private:
    xercesc::XercesDOMParser *m_parser;
};

#endif
