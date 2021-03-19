//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef DATABASETABLES_H
#define DATABASETABLES_H


const std::string TableDefinition_user = \
    "ID INTEGER PRIMARY KEY, \
     username VARCHAR(255),  \
     password VARCHAR(255),  \
     emailAddress VARCHAR(64) \
     ";

const std::string TableDefinition_domain = \
    "ID INTEGER PRIMARY KEY, \
     name VARCHAR(100) \
     ";

const std::string TableDefinition_domainParent = \
    "ID INTEGER PRIMARY KEY, \
     parentID INTEGER, \
     childID INTEGER, \
     FOREIGN KEY(parentID) REFERENCES domain(ID), \
     FOREIGN KEY(childID) REFERENCES domain(ID) \
     ";

const std::string TableDefinition_project = \
    "ID INTEGER PRIMARY KEY, \
     name VARCHAR(100) NOT NULL\
     ";

//  type : 'F' = filter, 'G' = testGroup &  'I' = testItem
const std::string TableDefinition_asset = \
    "ID INTEGER PRIMARY KEY, \
     name VARCHAR(100), \
     type TEXT CHECK( type IN ('F','G','I') ) NOT NULL DEFAULT 'I' \
     ";

const std::string TableDefinition_assetParent = \
    "ID INTEGER PRIMARY KEY, \
     parentID INTEGER NOT NULL, \
     childID INTEGER NOT NULL, \
     FOREIGN KEY(parentID) REFERENCES asset(ID), \
     FOREIGN KEY(childID) REFERENCES asset(ID) \
     ";

const std::string TableDefinition_projectAssetMap = \
    "ID INTEGER PRIMARY KEY, \
     projectID INTEGER NOT NULL, \
     assetID INTEGER, \
     FOREIGN KEY(projectID) REFERENCES project(ID), \
     FOREIGN KEY(assetID) REFERENCES asset(ID) \
     ";

//  type : 'F' = folder, 'R' = release &  'C' = cycle
const std::string TableDefinition_release = \
    "ID INTEGER PRIMARY KEY, \
     name VARCHAR(100), \
     type TEXT CHECK( type IN ('F','R','C') ) NOT NULL DEFAULT 'F' \
     ";

const std::string TableDefinition_projectReleaseMap = \
    "ID INTEGER PRIMARY KEY, \
     projectID INTEGER NOT NULL, \
     releaseID INTEGER, \
     FOREIGN KEY(projectID) REFERENCES project(ID), \
     FOREIGN KEY(releaseID) REFERENCES release(ID) \
     ";

#endif
