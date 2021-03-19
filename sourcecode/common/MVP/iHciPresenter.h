//  Copyright 2014 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef IHCIPRESENTATION_H
#define IHCIPRESENTATION_H

#include <MVP/iModel.h>
#include <MVP/iHciView.h>


class iHciPresenter
{
public:
    virtual ~iHciPresenter() {}

    virtual void AssignView( iHciView *view ) = 0;
};

#endif
