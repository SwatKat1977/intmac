//  Copyright 2014-2015 Integrated Test Management Centre Foundation Team
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <stringUtils.h>


namespace Common
{

namespace StringUtils
{

std::string StringToUppercase( const std::string &str )
{
    std::string strUpper = str;

    std::transform( strUpper.begin(),
                    strUpper.end(),
                    strUpper.begin(), toupper );

    return strUpper;
}

}

}
