//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef RELEASEFOLDERDETAILSPANE_H
#define RELEASEFOLDERDETAILSPANE_H

#include <wx/wx.h>


class ReleaseFolderDetailsPane : public wxPanel
{
public:
    ReleaseFolderDetailsPane( wxWindow *parent
                            , const wxSize &size );

    ~ReleaseFolderDetailsPane();
};

#endif
