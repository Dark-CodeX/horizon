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
#include "../errors/errors.hh"
#include "../misc/file/file.hh"
#include "./ast/ast.hh"

namespace horizon
{
    namespace horizon_parser
    {
        class parser
        {
          private:
            horizon_deps::vector<token> M_tokens;
            horizon_misc::HR_FILE *M_file;
            horizon_deps::sptr<ast_node> M_ast;

            std::size_t M_current_parser;

          private:
            [[nodiscard]] bool has_reached_end() const;
            [[nodiscard]] const token &pre_advance();
            const token &post_advance();
            [[nodiscard]] const token &get_token() const;

            [[nodiscard]] bool handle_semicolon();

            [[nodiscard]] horizon_deps::sptr<ast_node> parse_if_elif_else();

            [[nodiscard]] horizon_deps::sptr<ast_node> parse_block();

            [[nodiscard]] horizon_deps::sptr<ast_node> parse_variable_decl();

            [[nodiscard]] horizon_deps::sptr<ast_node> parse_operators();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_assignment_operator();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_logical_or();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_logical_and();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_bitwise_or();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_bitwise_xor();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_bitwise_and();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_equality_operator();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_relational_operator();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_bitwise_shift();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_expr();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_term();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_exponent();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_identifier();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_brackets();
            [[nodiscard]] horizon_deps::sptr<ast_node> parse_factor();

          public: // non-static public functions
            parser(horizon_deps::vector<token> &&movable_tokens, horizon_misc::HR_FILE *file);
            [[nodiscard]] bool init_parsing();
        };
    }
}

#endif