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
            return this->get_token().M_type == token_type::TOKEN_END_OF_FILE;
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

        bool parser::handle_semicolon()
        {
            if (this->get_token().M_type == token_type::TOKEN_SEMICOLON)
            {
                this->post_advance();
                return true;
            }
            else
            {
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ';', but got", this->get_token().M_lexeme});
                return false;
            }
        }

        horizon_deps::sptr<ast_node> parser::parse_do_while_loop()
        {
            horizon_deps::sptr<ast_node> block = nullptr;
            horizon_deps::sptr<ast_node> condition = nullptr;

            this->post_advance();
            block = this->parse_block();

            if (this->get_token().M_lexeme == "while")
            {
                this->post_advance();
                if (this->get_token().M_type != token_type::TOKEN_LEFT_PAREN)
                {
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '(' before '", this->get_token().M_lexeme, "'"});
                    return nullptr;
                }
                else
                    this->post_advance();
                if (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN)
                {
                    condition = this->parse_operators();
                    if (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN)
                    {
                        horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ')', but got", this->get_token().M_lexeme});
                        return nullptr;
                    }
                    this->post_advance();
                }
                else
                {
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected an expression before '", this->get_token().M_lexeme, "'"});
                    return nullptr;
                }
                return new ast_do_while_loop(std::move(block), std::move(condition));
            }
            else
            {
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected 'while', but got", this->get_token().M_lexeme});
                return nullptr;
            }
        }

        horizon_deps::sptr<ast_node> parser::parse_while_loop()
        {
            horizon_deps::sptr<ast_node> condition = nullptr;
            horizon_deps::sptr<ast_node> block = nullptr;

            this->post_advance();
            if (this->get_token().M_type != token_type::TOKEN_LEFT_PAREN)
            {
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '(' before '", this->get_token().M_lexeme, "'"});
                return nullptr;
            }
            else
                this->post_advance();
            if (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN)
            {
                condition = this->parse_operators();
                if (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN)
                {
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ')', but got", this->get_token().M_lexeme});
                    return nullptr;
                }
                this->post_advance();
            }
            else
            {
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected an expression before '", this->get_token().M_lexeme, "'"});
                return nullptr;
            }
            block = this->parse_block();
            return new ast_while_loop(std::move(condition), std::move(block));
        }

        horizon_deps::sptr<ast_node> parser::parse_for_loop()
        {
            horizon_deps::sptr<ast_node> variable_decl = nullptr;
            horizon_deps::sptr<ast_node> condition = nullptr;
            horizon_deps::sptr<ast_node> step = nullptr;
            horizon_deps::sptr<ast_node> block = nullptr;

            this->post_advance();
            if (this->get_token().M_type != token_type::TOKEN_LEFT_PAREN)
            {
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '(' before '", this->get_token().M_lexeme, "'"});
                return nullptr;
            }
            else
                this->post_advance();
            if (this->get_token().M_type != token_type::TOKEN_SEMICOLON)
            {
                variable_decl = this->parse_variable_decl();
                if (!this->handle_semicolon())
                    return nullptr;
            }
            else
                this->post_advance();
            if (this->get_token().M_type != token_type::TOKEN_SEMICOLON)
            {
                condition = this->parse_operators();
                if (!this->handle_semicolon())
                    return nullptr;
            }
            else
                this->post_advance();
            if (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN)
            {
                step = this->parse_operators();
                if (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN)
                {
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ')', but got", this->get_token().M_lexeme});
                    return nullptr;
                }
            }
            this->post_advance();
            block = this->parse_block();
            return new ast_for_loop(std::move(variable_decl), std::move(condition), std::move(step), std::move(block));
        }

        horizon_deps::sptr<ast_node> parser::parse_if_elif_else()
        {
            if (this->get_token().M_lexeme == "if")
            {
                horizon_deps::pair<horizon_deps::sptr<ast_node>> if_condition_block = {nullptr, nullptr};
                horizon_deps::vector<horizon_deps::pair<horizon_deps::sptr<ast_node>>> elif_condition_block;
                horizon_deps::sptr<ast_node> else_block = nullptr;

                {
                    // if block
                    this->post_advance();
                    if (this->get_token().M_type != token_type::TOKEN_LEFT_PAREN)
                    {
                        horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '(' before '", this->get_token().M_lexeme, "'"});
                        return nullptr;
                    }
                    if_condition_block.raw_first() = new horizon_deps::sptr<ast_node>(this->parse_operators());
                    if_condition_block.raw_second() = new horizon_deps::sptr<ast_node>(this->parse_block());
                }

                if (this->get_token().M_lexeme == "elif")
                {
                    while (this->get_token().M_lexeme == "elif")
                    {
                        horizon_deps::pair<horizon_deps::sptr<ast_node>> temp = {nullptr, nullptr};
                        this->post_advance();
                        if (this->get_token().M_type != token_type::TOKEN_LEFT_PAREN)
                        {
                            horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '(' before '", this->get_token().M_lexeme, "'"});
                            return nullptr;
                        }
                        temp.raw_first() = new horizon_deps::sptr<ast_node>(this->parse_operators());
                        temp.raw_second() = new horizon_deps::sptr<ast_node>(this->parse_block());
                        elif_condition_block.add(std::move(temp));
                    }
                    elif_condition_block.shrink_to_fit();
                }

                if (this->get_token().M_lexeme == "else")
                {
                    this->post_advance();
                    else_block = this->parse_block();
                }

                return new ast_if_elif_else(std::move(if_condition_block), std::move(elif_condition_block), std::move(else_block));
            }
            else
            {
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {horizon_deps::string("'") + this->get_token().M_lexeme + "' without a prior 'if'"});
                return nullptr;
            }
        }

        horizon_deps::sptr<ast_node> parser::parse_block()
        {
            if (this->get_token().M_type == token_type::TOKEN_LEFT_BRACE)
            {
                this->post_advance();
                horizon_deps::vector<horizon_deps::sptr<ast_node>> nodes;
                while (this->get_token().M_type != token_type::TOKEN_RIGHT_BRACE && !this->has_reached_end())
                {
                    // here, we need to parse statements, function calls, loops, decls and exprs
                    if (this->get_token().M_type == token_type::TOKEN_LEFT_BRACE)
                    {
                        nodes.add(this->parse_block());
                    }
                    else if (this->get_token().M_type == token_type::TOKEN_KEYWORD)
                    {
                        if (this->get_token().M_lexeme == "if" || this->get_token().M_lexeme == "elif" || this->get_token().M_lexeme == "else")
                            nodes.add(this->parse_if_elif_else());
                        else if (this->get_token().M_lexeme == "for")
                            nodes.add(this->parse_for_loop());
                        else if (this->get_token().M_lexeme == "while")
                            nodes.add(this->parse_while_loop());
                        else if (this->get_token().M_lexeme == "do")
                            nodes.add(this->parse_do_while_loop());
                    }
                    else if (this->get_token().M_type == token_type::TOKEN_IDENTIFIER && (this->M_tokens[this->M_current_parser + 1].M_type != token_type::TOKEN_COLON || this->M_tokens[this->M_current_parser + 1].M_type == token_type::TOKEN_LEFT_PAREN))
                    {
                        // expr, function calls, statements
                        horizon_deps::sptr<ast_node> x1;
                        do
                        {
                            x1 = this->parse_operators();
                            if (!this->handle_semicolon())
                                return nullptr;
                            nodes.add(std::move(x1));
                        } while (x1);
                    }
                    else
                    {
                        // var decl
                        horizon_deps::sptr<ast_node> x2;
                        do
                        {
                            x2 = this->parse_variable_decl();
                            if (!this->handle_semicolon())
                                return nullptr;
                            nodes.add(std::move(x2));
                        } while (x2 && (this->get_token().M_type == token_type::TOKEN_PRIMARY_TYPE || this->get_token().M_type == token_type::TOKEN_IDENTIFIER) && this->M_tokens[this->M_current_parser + 1].M_type == token_type::TOKEN_COLON);
                    }
                }
                if (this->get_token().M_type != token_type::TOKEN_RIGHT_BRACE)
                {
                    return nullptr;
                }
                else
                    this->post_advance();
                nodes.shrink_to_fit();
                return new ast_block(std::move(nodes));
            }
            else
                return this->parse_variable_decl();
        }

        horizon_deps::sptr<ast_node> parser::parse_variable_decl()
        {
            if (this->get_token().M_type == token_type::TOKEN_PRIMARY_TYPE || this->get_token().M_type == token_type::TOKEN_IDENTIFIER)
            {
                horizon_deps::vector<horizon_deps::pair<horizon_deps::string, horizon_deps::sptr<ast_node>>> vec(5);
                horizon_deps::string &type = this->M_tokens[this->M_current_parser++].M_lexeme;
                if (this->get_token().M_type != token_type::TOKEN_COLON)
                {
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ':' after data type", type});
                    return nullptr;
                }
                this->post_advance();
                while (this->get_token().M_type != token_type::TOKEN_SEMICOLON && !this->has_reached_end())
                {
                    horizon_deps::pair<horizon_deps::string, horizon_deps::sptr<ast_node>> pair;
                    if (this->get_token().M_type == token_type::TOKEN_PRIMARY_TYPE || this->get_token().M_type == token_type::TOKEN_KEYWORD)
                    {
                        horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"reserved word", this->get_token().M_lexeme, "cannot be used as an identifier"});
                        return nullptr;
                    }
                    else if (this->get_token().M_type == token_type::TOKEN_IDENTIFIER)
                        pair.raw_first() = new horizon_deps::string(this->get_token().M_lexeme);
                    else
                    {
                        horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected an identifier, but got", this->get_token().M_lexeme});
                        return nullptr;
                    }
                    this->post_advance();
                    if (this->get_token().M_type == token_type::TOKEN_ASSIGN)
                    {
                        this->post_advance();
                        pair.raw_second() = new horizon_deps::sptr<ast_node>(this->parse_operators());
                        if (this->get_token().M_type == token_type::TOKEN_COMMA)
                            this->post_advance();
                    }
                    else if (this->get_token().M_type == token_type::TOKEN_COMMA)
                    {
                        this->post_advance();
                        pair.raw_second() = nullptr;
                    }
                    vec.add(std::move(pair));
                }
                vec.shrink_to_fit();
                return new ast_variable_declaration(std::move(type), std::move(vec));
            }
            return nullptr;
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
            horizon_deps::sptr<ast_node> left = this->parse_member_access();
            while (this->get_token().M_type == token_type::TOKEN_ARITHMETIC_POWER)
            {
                const token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_member_access();
                left = new ast_binary_operation_node(std::move(left), operator_token.M_type, std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_member_access()
        {
            horizon_deps::sptr<ast_node> left = this->parse_identifier();
            while (this->get_token().M_type == token_type::TOKEN_DOT)
            {
                const token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_identifier();
                left = new ast_binary_operation_node(std::move(left), operator_token.M_type, std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_identifier()
        {
            if (this->get_token().M_type == token_type::TOKEN_IDENTIFIER)
            {
                horizon_deps::string identifier = this->post_advance().M_lexeme;
                if (this->get_token().M_type == token_type::TOKEN_LEFT_PAREN)
                {
                    this->post_advance();
                    horizon_deps::vector<horizon_deps::sptr<ast_node>> vec(5);
                    while (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN && !this->has_reached_end())
                    {
                        vec.add(this->parse_operators());
                        if (this->get_token().M_type == token_type::TOKEN_COMMA)
                            this->post_advance();
                    }
                    if (this->get_token().M_type == token_type::TOKEN_RIGHT_PAREN)
                        this->post_advance();
                    vec.shrink_to_fit();
                    return new ast_function_call(std::move(identifier), std::move(vec));
                }
                else if (this->get_token().M_type == token_type::TOKEN_INCREMENT || this->get_token().M_type == token_type::TOKEN_DECREMENT)
                {
                    return new ast_post_unary_operation_node(std::move(identifier), this->post_advance().M_type);
                }
                else
                    return new ast_operand_node(std::move(identifier));
            }
            else
                return this->parse_brackets();
        }

        horizon_deps::sptr<ast_node> parser::parse_brackets()
        {
            if (this->get_token().M_type == token_type::TOKEN_LEFT_PAREN)
            {
                this->post_advance();
                horizon_deps::sptr<ast_node> x = this->parse_operators();
                if (this->get_token().M_type == token_type::TOKEN_RIGHT_PAREN)
                    this->post_advance();
                return x;
            }
            else
                return this->parse_factor();
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
            else if (this->get_token().M_type == token_type::TOKEN_STRING_LITERAL)
            {
                return new ast_operand_node(this->post_advance().M_lexeme.c_str());
            }
            else if (this->get_token().M_type == token_type::TOKEN_CHAR_LITERAL)
            {
                return new ast_operand_node(*this->post_advance().M_lexeme.c_str());
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
                this->M_ast = this->parse_do_while_loop();
                if (!this->M_ast)
                    return false;
                this->M_ast->print();
            }
            this->M_tokens.erase();
            return true;
        }
    }
}