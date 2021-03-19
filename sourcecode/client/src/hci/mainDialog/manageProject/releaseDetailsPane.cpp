//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <hci/MainDialog/manageProject/releaseDetailsPane.h>


ReleaseDetailsPane::ReleaseDetailsPane( wxWindow *parent
                                      , const wxSize &size )
                  : wxPanel( parent
                           , wxID_ANY
                           , wxDefaultPosition
                           , size )
{
    new wxStaticText(this, 121, "ReleaseDetailsPane");
}


ReleaseDetailsPane::~ReleaseDetailsPane()
{
}
