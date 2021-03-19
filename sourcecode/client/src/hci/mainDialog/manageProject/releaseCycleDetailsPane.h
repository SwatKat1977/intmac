//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef RELEASECYCLEDETAILSPANE_H
#define RELEASECYCLEDETAILSPANE_H

#include <wx/wx.h>


class ReleaseCycleDetailsPane : public wxPanel
{
public:
    ReleaseCycleDetailsPane( wxWindow *parent
                           , const wxSize &size );

    ~ReleaseCycleDetailsPane();
};

#endif
