/**
 * @file parser.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_PARSER_PARSER_HH
#define HORIZON_PARSER_PARSER_HH

#include "../../deps/string/string.hh"
#include "../../deps/vector/vector.hh"
#include "../token_type/token_type.hh"
#include "../token/token.hh"
#include "../defines/keywords_primary_data_types.h"
#include "./function_declaration/function_declaration.hh"
#include "../errors/errors.hh"
#include "../misc/file/file.hh"

namespace horizon
{
    namespace horizon_parser
    {
        class parser
        {
        private:
            horizon_deps::vector<token> M_tokens;
            horizon_misc::HR_FILE *M_file;

            std::size_t M_current_parser;

        private:
            [[nodiscard]] bool has_reached_end() const;

            [[nodiscard]] bool parse_function();

            [[nodiscard]] bool read_tokens();

        public: // non-static public functions
            parser(horizon_deps::vector<token> &&movable_vec, horizon_misc::HR_FILE *file);
            [[nodiscard]] bool init_parsing();

            void debug_print() const;
        };
    }
}

#endif