/**
 * @file file.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_MISC_FILE_FILE_HH
#define HORIZON_MISC_FILE_FILE_HH

#include "../../../deps/string/string.hh"

namespace horizon
{
    namespace horizon_misc
    {
        struct HR_FILE
        {
            horizon_deps::string M_location;
            horizon_deps::string M_content;
        };
    }
};

#endif