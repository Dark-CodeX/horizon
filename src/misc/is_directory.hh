/**
 * @file is_directory.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_MISC_IS_DIRECTORY_HH
#define HORIZON_MISC_IS_DIRECTORY_HH

#include <cstdio>
#include <cerrno>
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#include <Windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

namespace horizon
{
    namespace horizon_misc
    {
        [[nodiscard]] bool is_directory(const char *loc);
    }
}

#endif