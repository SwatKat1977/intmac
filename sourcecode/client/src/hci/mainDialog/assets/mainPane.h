//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef MAINPANE_H
#define MAINPANE_H

#include <wx/wx.h>


const wxWindowID ID_mainPane = 0x030;


class MainPane : public wxPanel
{
public:
    MainPane( wxWindow *parent );

    ~MainPane();
};


#endif
