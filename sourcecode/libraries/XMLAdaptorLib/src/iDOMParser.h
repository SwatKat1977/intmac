//  Copyright 2015 Paul Morriss
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IDOMPARSER_H
#define IDOMPARSER_H

#include <iDOMDocument.h>


enum ValSchemes
{
    Val_Never,
    Val_Always,
    Val_Auto
};


enum GrammarType 
{
    DTDGrammarType, 
    SchemaGrammarType, 
    UnKnown
};


/* *********************************************************************** */
/*                         iDOMParser Class Definition                     */
/* *********************************************************************** */
class iDOMParser
{
public:
    virtual ~iDOMParser( ) { }

    //  Getter methods
    virtual iDOMDocument *getDocument( ) = 0;
    virtual const size_t getErrorCount( ) = 0;

    //  Setter methods
    virtual void setDoNamespaces( bool newState ) = 0;
    virtual void setDoSchema( bool newState ) = 0;
    virtual void SetErrorHandler( ) = 0;
    virtual void setValidationConstraintFatal( bool newState ) = 0;
    virtual void setValidationScheme( ValSchemes newScheme ) = 0;

    //  Parsing methods
    virtual void parseFile( std::string filename ) = 0;
    virtual void parseString( std::string xmlString
                            , size_t stringSize ) = 0;

    virtual bool loadGrammar( std::string schemaName, 
                              GrammarType grammarType,
                              bool toCache = false ) = 0;

};

#endif
