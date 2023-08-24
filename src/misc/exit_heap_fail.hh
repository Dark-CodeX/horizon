/**
 * @file exit_heap_fail.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_MISC_EXIT_HEAP_FAIL_HH
#define HORIZON_MISC_EXIT_HEAP_FAIL_HH

#include <cstdio>
#include <cerrno>

#include "../colorize/colorize.h"

namespace horizon
{
    namespace horizon_misc
    {
        void exit_heap_fail(const void *ptr, const char *__s);
    }
}

#endif