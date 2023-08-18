/**
 * @file misc.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_MISC_MISC_HH
#define HORIZON_MISC_MISC_HH

#include <cstdio>
#include <cerrno>

#include "./file/file.hh"
#include "../../deps/string/string.hh"
#include "../colorize/colorize.h"
#include "../defines/defines.h"

namespace horizon
{
    namespace horizon_misc
    {
        class misc
        {
        public:
            [[nodiscard]] static HR_FILE *load_file(const char *loc);
            static void exit_heap_fail(const void *ptr, const char *__s);
        };
    }
}

#endif