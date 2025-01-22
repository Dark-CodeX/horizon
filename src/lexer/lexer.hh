/**
 * @file lexer.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_LEXER_LEXER_HH
#define HORIZON_LEXER_LEXER_HH

#include <cerrno>
#include <cctype>
#include <stack>

#include "../../deps/string/string.hh"
#include "../../deps/vector/vector.hh"
#include "../defines/defines.h"
#include "../defines/keywords_primary_data_types.h"
#include "../token/token.hh"
#include "../token_type/token_type.hh"
#include "../errors/errors.hh"
#include "../colorize/colorize.h"
#include "../misc/file/file.hh"

namespace horizon
{
    namespace horizon_lexer
    {
        class lexer
        {
        private:                                  // non-static class variables
            horizon_deps::vector<token> M_tokens; // contains tokens

            horizon_misc::HR_FILE *M_file;

            std::size_t M_line; // current line, just for reporting errors

            std::size_t M_current_lexer; // current lexer index
            std::size_t M_start_lexer;   // start lexer index

            char M_ch;         // current character
            char M_invalid_ec; // stores any invalid escape seq.

        private:
            [[nodiscard]] bool scan_tokens();
            void append_token(const token_type &type);

            [[nodiscard]] bool has_reached_eof() const;

            /**
             * @brief First reads the character to M_ch and then increment M_current_lexer by 1
             * @brief Manages M_line and M_col
             */
            void post_advance_lexer();

            /**
             * @brief First increment M_current_lexer by 1 and then reads the character to M_ch
             * @brief Manages M_line and M_col
             */
            void pre_advance_lexer();

            /**
             * @brief Does not increment M_current_lexer, it just reads the character to M_ch at M_current_lexer
             * @brief Manages M_line and M_col ONLY if previous character (i.e., before reading) is NOT equal to current M_ch
             */
            void load_char();

            bool handle_integer_decimal();
            void handle_identifier();
            [[nodiscard]] horizon_errors::error_code handle_comments();
            [[nodiscard]] horizon_errors::error_code handle_char();
            [[nodiscard]] horizon_errors::error_code handle_string();

            [[nodiscard]] std::size_t check_brackets() const;

        public: // non-static class functions
            lexer(horizon_misc::HR_FILE *file);

            [[nodiscard]] bool init_lexing();

            [[nodiscard]] const horizon_deps::vector<token> &get() const;
            [[nodiscard]] horizon_deps::vector<token> &get();
            [[nodiscard]] horizon_deps::vector<token> &&move();

            void debug_print() const;
        };
    }
}

#endif