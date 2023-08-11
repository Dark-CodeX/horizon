/**
 * @file lexer_errors.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_LEXER_LEXER_ERRORS_HH
#define HORIZON_LEXER_LEXER_ERRORS_HH

#include <utility>

#include "../../../deps/string/string.hh"
#include "../../../deps/vector/vector.hh"
#include "../../colorize/colorize.h"

namespace horizon
{
    namespace horizon_lexer
    {
        enum class lexer_errors_code : unsigned char
        {
            HORIZON_IO_ERROR,
            HORIZON_UNKNOWN_TOKEN,
            HORIZON_INVALID_ESCAPE_SEQUENCE,
            HORIZON_MISSING_TERMINATING_CHAR,
            HORIZON_LESS_BYTES,
            HORIZON_MORE_BYTES,
            HORIZON_INVALID_BRACKET,
            HORIZON_NO_ERROR
        };

        class lexer_errors
        {
        public:
            [[nodiscard]] static std::pair<horizon_deps::string, std::size_t> getline(const horizon_deps::string &str, const std::size_t &start, const std::size_t &end__, const horizon_deps::string &color);

            [[nodiscard]] static std::size_t getline_no(const horizon_deps::string &str, const std::size_t &start);

            static void draw_error(const lexer_errors_code &code, const horizon_deps::string &file_loc, const std::size_t &line_no, const std::size_t &start, const std::size_t &end, const horizon_deps::vector<horizon_deps::string> &err_msg, const horizon_deps::string &file);
        };
    }
}

#endif