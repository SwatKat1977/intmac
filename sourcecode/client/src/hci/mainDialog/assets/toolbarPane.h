//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#ifndef TOOLBARPANE_H
#define TOOLBARPANE_H

#include <wx/wx.h>
#include <hci/mainDialog/assets/assetsTreeCtrl.h>
#include <dataModels/assetDataTransaction.h>


const wxWindowID ID_toolbarPane = 0x010;


class ToolbarPane : public wxPanel
{
public:
    ToolbarPane( wxWindow *parent
               , const wxSize &size );

    ~ToolbarPane();
 
    void AddAssetItem( AssetDataTransaction * transaction );

private:
    AssetsTreeCtrl *m_treeCtrl;
};


#endif
