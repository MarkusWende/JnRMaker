/****************************************************************************
 * Copyright (C) 2019 Kirsch-Audio GmbH <software@kirsch-audio.com>
 * All rights reserved.
 *
 * This file is part of ANCMonitor
 *
 *	SoundIMP is a software for creating sound immission predictions.
 *
 *	Unauthorized copying of this file, via any medium is strictly
 *	prohibited
 *
 *	Proprietary and confidential.
 ****************************************************************************/

/**
 * @file FormatHelper.h
 * @author Markus Wende
 * @brief Format helper namespace.
 */


#pragma once

#include <iostream>
#include <sstream>
#include <stdarg.h>

namespace FormatHelper
{
    inline std::string vformat(const char* format, va_list args)
    {
        va_list args_copy;
        va_copy(args_copy, args);

        int size = std::vsnprintf(NULL, 0, format, args);
        std::string buffer(size, '\0');

        std::vsnprintf(&buffer[0], buffer.size() + 1, format, args_copy);
        va_end(args_copy);

        return buffer;
    }
}