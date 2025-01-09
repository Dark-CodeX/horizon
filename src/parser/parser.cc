/**
 * @file parser.cc
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#include "./parser.hh"

namespace horizon
{
    namespace horizon_parser
    {
        bool parser::has_reached_end() const
        {
            return this->M_current_parser >= this->M_tokens.length();
        }

        const token &parser::pre_advance()
        {
            return this->M_tokens[++this->M_current_parser];
        }

        const token &parser::post_advance()
        {
            return this->M_tokens[this->M_current_parser++];
        }

        const token &parser::get_token() const
        {
            return this->M_tokens[this->M_current_parser];
        }

        horizon_deps::sptr<ast_node> parser::parse_operators()
        {
            return this->parse_assignment_operator();
        }

        horizon_deps::sptr<ast_node> parser::parse_assignment_operator()
        {
            horizon_deps::sptr<ast_node> left = this->parse_logical_or();
            while (this->get_token().M_type == token_type::TOKEN_ASSIGN ||
                   this->get_token().M_type == token_type::TOKEN_ASSIGN_ADD ||
                   this->get_token().M_type == token_type::TOKEN_ASSIGN_SUBSTRACT ||
                   this->get_token().M_type == token_type::TOKEN_ASSIGN_MULTIPLY ||
                   this->get_token().M_type == token_type::TOKEN_ASSIGN_POWER ||
                   this->get_token().M_type == token_type::TOKEN_ASSIGN_DIVIDE ||
                   this->get_token().M_type == token_type::TOKEN_ASSIGN_MODULUS ||
                   this->get_token().M_type == token_type::TOKEN_ASSIGN_BITWISE_AND ||
                   this->get_token().M_type == token_type::TOKEN_ASSIGN_BITWISE_OR ||
                   this->get_token().M_type == token_type::TOKEN_ASSIGN_BITWISE_XOR ||
                   this->get_token().M_type == token_type::TOKEN_ASSIGN_LEFT_SHIFT ||
                   this->get_token().M_type == token_type::TOKEN_ASSIGN_RIGHT_SHIFT)
            {
                const token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_logical_or();
                left = new ast_binary_operation_node(std::move(left), operator_token.M_type, std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_logical_or()
        {
            horizon_deps::sptr<ast_node> left = this->parse_logical_and();
            while (this->get_token().M_type == token_type::TOKEN_LOGICAL_OR)
            {
                const token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_logical_and();
                left = new ast_binary_operation_node(std::move(left), operator_token.M_type, std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_logical_and()
        {
            horizon_deps::sptr<ast_node> left = this->parse_bitwise_or();
            while (this->get_token().M_type == token_type::TOKEN_LOGICAL_AND)
            {
                const token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_bitwise_or();
                left = new ast_binary_operation_node(std::move(left), operator_token.M_type, std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_bitwise_or()
        {
            horizon_deps::sptr<ast_node> left = this->parse_bitwise_xor();
            while (this->get_token().M_type == token_type::TOKEN_BITWISE_OR)
            {
                const token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_bitwise_xor();
                left = new ast_binary_operation_node(std::move(left), operator_token.M_type, std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_bitwise_xor()
        {
            horizon_deps::sptr<ast_node> left = this->parse_bitwise_and();
            while (this->get_token().M_type == token_type::TOKEN_BITWISE_XOR)
            {
                const token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_bitwise_and();
                left = new ast_binary_operation_node(std::move(left), operator_token.M_type, std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_bitwise_and()
        {
            horizon_deps::sptr<ast_node> left = this->parse_equality_operator();
            while (this->get_token().M_type == token_type::TOKEN_BITWISE_AND)
            {
                const token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_equality_operator();
                left = new ast_binary_operation_node(std::move(left), operator_token.M_type, std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_equality_operator()
        {
            horizon_deps::sptr<ast_node> left = this->parse_relational_operator();
            while (this->get_token().M_type == token_type::TOKEN_RELATIONAL_EQUAL_TO ||
                   this->get_token().M_type == token_type::TOKEN_RELATIONAL_NOT_EQUAL_TO)
            {
                const token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_relational_operator();
                left = new ast_binary_operation_node(std::move(left), operator_token.M_type, std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_relational_operator()
        {
            horizon_deps::sptr<ast_node> left = this->parse_bitwise_shift();
            while (this->get_token().M_type == token_type::TOKEN_RELATIONAL_GREATER_THAN ||
                   this->get_token().M_type == token_type::TOKEN_RELATIONAL_LESS_THAN ||
                   this->get_token().M_type == token_type::TOKEN_RELATIONAL_GREATER_THAN_OR_EQUAL_TO ||
                   this->get_token().M_type == token_type::TOKEN_RELATIONAL_LESS_THAN_OR_EQUAL_TO)
            {
                const token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_bitwise_shift();
                left = new ast_binary_operation_node(std::move(left), operator_token.M_type, std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_bitwise_shift()
        {
            horizon_deps::sptr<ast_node> left = this->parse_expr();
            while (this->get_token().M_type == token_type::TOKEN_BITWISE_LEFT_SHIFT ||
                   this->get_token().M_type == token_type::TOKEN_BITWISE_RIGHT_SHIFT)
            {
                const token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_expr();
                left = new ast_binary_operation_node(std::move(left), operator_token.M_type, std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_expr()
        {
            horizon_deps::sptr<ast_node> left = this->parse_term();
            while (this->get_token().M_type == token_type::TOKEN_ARITHMETIC_ADD ||
                   this->get_token().M_type == token_type::TOKEN_ARITHMETIC_SUBSTRACT)
            {
                const token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_term();
                left = new ast_binary_operation_node(std::move(left), operator_token.M_type, std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_term()
        {
            horizon_deps::sptr<ast_node> left = this->parse_exponent();
            while (this->get_token().M_type == token_type::TOKEN_ARITHMETIC_MULTIPLY ||
                   this->get_token().M_type == token_type::TOKEN_ARITHMETIC_DIVIDE ||
                   this->get_token().M_type == token_type::TOKEN_ARITHMETIC_MODULUS)
            {
                const token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_exponent();
                left = new ast_binary_operation_node(std::move(left), operator_token.M_type, std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_exponent()
        {
            horizon_deps::sptr<ast_node> left = this->parse_factor();
            while (this->get_token().M_type == token_type::TOKEN_ARITHMETIC_POWER)
            {
                const token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_factor();
                left = new ast_binary_operation_node(std::move(left), operator_token.M_type, std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_factor()
        {
            if (this->get_token().M_type == token_type::TOKEN_DECIMAL_LITERAL)
            {
                return new ast_operand_node(std::strtold(this->post_advance().M_lexeme.c_str(), NULL));
            }
            else if (this->get_token().M_type == token_type::TOKEN_INTEGER_LITERAL)
            {
                return new ast_operand_node(std::strtoll(this->post_advance().M_lexeme.c_str(), NULL, 10));
            }
            else if (this->get_token().M_type == token_type::TOKEN_IDENTIFIER)
            {
                return new ast_operand_node(this->post_advance().M_lexeme);
            }
            else
            {
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"unexpected token", this->get_token().M_lexeme});
                return nullptr;
            }
        }

        parser::parser(horizon_deps::vector<token> &&movable_tokens, horizon_misc::HR_FILE *file)
        {
            this->M_tokens = std::move(movable_tokens);
            this->M_file = file;

            this->M_current_parser = 0;
        }

        bool parser::init_parsing()
        {
            while (!this->has_reached_end())
            {
                this->M_ast = this->parse_operators();
                if (!this->M_ast)
                    return false;
                this->M_ast->print();
                break;
            }
            this->M_tokens.erase();
            return true;
        }
    }
}