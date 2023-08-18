/**
 * @file errors.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_ERRORS_ERRORS_HH
#define HORIZON_ERRORS_ERRORS_HH

#include <utility>

#include "../../deps/string/string.hh"
#include "../../deps/vector/vector.hh"
#include "../colorize/colorize.h"
#include "../defines/defines.h"
#include "../token/token.hh"
#include "../misc/file/file.hh"

namespace horizon
{
    namespace horizon_errors
    {
        enum class error_code : unsigned
        {
            HORIZON_IO_ERROR = 1U,
            HORIZON_UNKNOWN_TOKEN,
            HORIZON_INVALID_ESCAPE_SEQUENCE,
            HORIZON_MISSING_TERMINATING_CHAR,
            HORIZON_LESS_BYTES,
            HORIZON_MORE_BYTES,
            HORIZON_INVALID_BRACKET,

            HORIZON_SYNTAX_ERROR = 10U,

            HORIZON_NO_ERROR
        };

        class errors
        {
        public:
            [[nodiscard]] static std::pair<horizon_deps::string, std::size_t> getline(const horizon_deps::string &str, const std::size_t &start, const std::size_t &end__, const horizon_deps::string &color);

            [[nodiscard]] static std::size_t getline_no(const horizon_deps::string &str, const std::size_t &start);

            static void lexer_draw_error(const error_code &code, const horizon_misc::HR_FILE *file, const std::size_t &line_no, const std::size_t &start, const std::size_t &end, const horizon_deps::vector<horizon_deps::string> &err_msg);

            static void parser_draw_error(const error_code &code, const horizon_misc::HR_FILE *file, const token &tok, const horizon_deps::vector<horizon_deps::string> &err_msg);
        };
    }
}

#endif