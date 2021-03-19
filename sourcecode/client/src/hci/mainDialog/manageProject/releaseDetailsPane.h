//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef RELEASEDETAILSPANE_H
#define RELEASEDETAILSPANE_H


#include <wx/wx.h>


class ReleaseDetailsPane : public wxPanel
{
public:
    ReleaseDetailsPane( wxWindow *parent
                      , const wxSize &size );

    ~ReleaseDetailsPane();
};

#endif
