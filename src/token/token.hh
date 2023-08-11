/**
 * @file token.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_TOKEN_TOKEN_HH
#define HORIZON_TOKEN_TOKEN_HH

#include "../../deps/string/string.hh"
#include "../token_type/token_type.hh"

namespace horizon
{
    struct token
    {
    public:
        token_type M_type;
        horizon_deps::string M_lexeme;
        std::size_t M_start;
        std::size_t M_end;
    };
}

#endif