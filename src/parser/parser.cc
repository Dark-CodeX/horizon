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

        token &parser::post_advance()
        {
            return this->M_tokens[this->M_current_parser++];
        }

        const token &parser::get_token() const
        {
            return this->M_tokens[this->M_current_parser];
        }

        token &parser::get_token()
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
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ';', but got", this->get_token().M_lexeme.wrap("'")});
                return false;
            }
        }

        void parser::handle_eof()
        {
            if (this->get_token().M_type == token_type::TOKEN_END_OF_FILE)
                this->M_current_parser--;
        }

        horizon_deps::sptr<ast_node> parser::parse_program()
        {
            horizon_deps::vector<horizon_deps::sptr<ast_node>> nodes;
            while (!this->has_reached_end())
            {
                horizon_deps::sptr<ast_node> temp = nullptr;
                if (this->get_token().M_lexeme == "func")
                    temp = this->parse_function();
                else
                {
                    // global varibales
                    temp = this->parse_variable_decl();
                    if (!this->handle_semicolon())
                        return nullptr;
                }
                if (!temp)
                    return nullptr;
                nodes.add(std::move(temp));
            }
            nodes.shrink_to_fit();
            return new ast_program_node(std::move(nodes));
        }

        horizon_deps::sptr<ast_node> parser::parse_data_type()
        {
            horizon_deps::vector<token> type_qualifiers;
            horizon_deps::sptr<ast_node> _type = nullptr;

            if (this->get_token().M_type == token_type::TOKEN_KEYWORD)
            {
                while (this->get_token().M_lexeme == "const" ||
                       this->get_token().M_lexeme == "ref" ||
                       this->get_token().M_lexeme == "static")
                {
                    type_qualifiers.add(std::move(this->post_advance()));
                }
                type_qualifiers.shrink_to_fit();
            }
            if (this->get_token().M_type == token_type::TOKEN_IDENTIFIER)
            {
                _type = this->parse_member_access();
                if (!_type)
                    return nullptr;
            }
            else if (this->get_token().M_type == token_type::TOKEN_PRIMARY_TYPE || this->get_token().M_lexeme == "let")
            {
                _type = new ast_operand_node(this->post_advance().M_lexeme);
            }
            else
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected an identifier, but got", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            return new ast_data_type_node(std::move(type_qualifiers), std::move(_type));
        }

        horizon_deps::sptr<ast_node> parser::parse_parameters()
        {
            horizon_deps::vector<horizon_deps::pair<horizon_deps::sptr<ast_node>, horizon_deps::vector<horizon_deps::pair<token, horizon_deps::sptr<ast_node>>>>> params;
            while (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN && !this->has_reached_end())
            {
                horizon_deps::pair<horizon_deps::sptr<ast_node>, horizon_deps::vector<horizon_deps::pair<token, horizon_deps::sptr<ast_node>>>> temp_pair1;
                temp_pair1.raw_first() = new horizon_deps::sptr<ast_node>(this->parse_data_type());
                if (!(*temp_pair1.raw_first()))
                    return nullptr;
                if (this->get_token().M_type != token_type::TOKEN_COLON)
                {
                    this->handle_eof();
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ':', but got", this->get_token().M_lexeme.wrap("'")});
                    return nullptr;
                }
                this->post_advance();
                horizon_deps::vector<horizon_deps::pair<token, horizon_deps::sptr<ast_node>>> temp_vec;
                while (this->get_token().M_type != token_type::TOKEN_COMMA && this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN && !this->has_reached_end())
                {
                    horizon_deps::pair<token, horizon_deps::sptr<ast_node>> temp_pair2;
                    std::size_t temp_curr_parser = this->M_current_parser;
                    bool is_data_type = false;
                    while (this->get_token().M_type != token_type::TOKEN_COMMA && this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN && !this->has_reached_end())
                    {
                        if (this->post_advance().M_type == token_type::TOKEN_COLON)
                        {
                            is_data_type = true;
                            break;
                        }
                    }
                    this->M_current_parser = temp_curr_parser;
                    if (is_data_type)
                        break;
                    if (this->get_token().M_type == token_type::TOKEN_PRIMARY_TYPE || this->get_token().M_type == token_type::TOKEN_KEYWORD)
                    {
                        this->handle_eof();
                        horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"reserved word", this->get_token().M_lexeme.wrap("'"), "cannot be used as an identifier"});
                        return nullptr;
                    }
                    else if (this->get_token().M_type == token_type::TOKEN_IDENTIFIER)
                        temp_pair2.raw_first() = new token(std::move(this->get_token()));
                    else
                    {
                        this->handle_eof();
                        horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected an identifier, but got", this->get_token().M_lexeme.wrap("'")});
                        return nullptr;
                    }
                    this->post_advance();
                    if (this->get_token().M_type == token_type::TOKEN_ASSIGN)
                    {
                        this->post_advance();
                        temp_pair2.raw_second() = new horizon_deps::sptr<ast_node>(this->parse_operators());
                        if (!(*temp_pair2.raw_second()))
                            return nullptr;
                        if (this->get_token().M_type == token_type::TOKEN_COMMA)
                            this->post_advance();
                        else if (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN)
                        {
                            this->handle_eof();
                            horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"unexpected token", this->get_token().M_lexeme.wrap("'"), "expected ')'"});
                            return nullptr;
                        }
                    }
                    else if (this->get_token().M_type == token_type::TOKEN_COMMA)
                    {
                        this->post_advance();
                        temp_pair2.raw_second() = nullptr;
                    }
                    else if (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN)
                    {
                        this->handle_eof();
                        horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"unexpected token", this->get_token().M_lexeme.wrap("'"), "expected ')'"});
                        return nullptr;
                    }
                    temp_vec.add(std::move(temp_pair2));
                }
                temp_vec.shrink_to_fit();
                temp_pair1.raw_second() = new horizon_deps::vector<horizon_deps::pair<token, horizon_deps::sptr<ast_node>>>(std::move(temp_vec));
                params.add(std::move(temp_pair1));
            }
            params.shrink_to_fit();
            return new ast_parameter_node(std::move(params));
        }

        horizon_deps::sptr<ast_node> parser::parse_function()
        {
            if (this->get_token().M_lexeme != "func")
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected 'func', but got", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            token identifier;
            horizon_deps::sptr<ast_node> parameters = nullptr;
            horizon_deps::sptr<ast_node> return_type = nullptr;
            horizon_deps::sptr<ast_node> block = nullptr;

            this->post_advance();
            if (this->get_token().M_type == token_type::TOKEN_PRIMARY_TYPE || this->get_token().M_type == token_type::TOKEN_KEYWORD)
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"reserved word", this->get_token().M_lexeme.wrap("'"), "cannot be used as an identifier"});
                return nullptr;
            }
            else if (this->get_token().M_type == token_type::TOKEN_IDENTIFIER)
                identifier = std::move(this->get_token());
            else
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected an identifier, but got", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            this->post_advance();
            if (this->get_token().M_type != token_type::TOKEN_LEFT_PAREN)
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '(' before", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            else
                this->post_advance();
            if (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN)
            {
                parameters = this->parse_parameters();
                if (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN)
                {
                    this->handle_eof();
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ')', but got", this->get_token().M_lexeme.wrap("'")});
                    return nullptr;
                }
            }
            this->post_advance();
            if (this->get_token().M_type != token_type::TOKEN_COLON)
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ':', but got", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            this->post_advance();
            return_type = this->parse_data_type();
            if (!return_type)
                return nullptr;
            if (this->get_token().M_type != token_type::TOKEN_LEFT_BRACE)
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '{', but got", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            block = this->parse_block();
            if (!block)
                return nullptr;
            return new ast_function_declaration_node(std::move(identifier), std::move(parameters), std::move(return_type), std::move(block));
        }

        horizon_deps::sptr<ast_node> parser::parse_jump_statements()
        {
            token keyword_;
            horizon_deps::sptr<ast_node> expr = nullptr;
            if (this->get_token().M_lexeme == "break" || this->get_token().M_lexeme == "continue")
            {
                keyword_ = std::move(this->post_advance());
                if (!this->handle_semicolon())
                    return nullptr;
            }
            else if (this->get_token().M_lexeme == "return")
            {
                keyword_ = std::move(this->post_advance());
                if (this->get_token().M_type == token_type::TOKEN_SEMICOLON)
                {
                    this->post_advance();
                }
                else
                {
                    expr = this->parse_operators();
                    if (!this->handle_semicolon())
                        return nullptr;
                }
            }
            return new ast_jump_statement_node(std::move(keyword_), std::move(expr));
        }

        horizon_deps::sptr<ast_node> parser::parse_do_while_loop()
        {
            if (this->get_token().M_lexeme != "do")
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected 'do', but got", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            horizon_deps::sptr<ast_node> block = nullptr;
            horizon_deps::sptr<ast_node> condition = nullptr;

            this->post_advance();
            if (this->get_token().M_type != token_type::TOKEN_LEFT_BRACE)
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '{', but got", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            block = this->parse_block();
            if (!block)
                return nullptr;

            if (this->get_token().M_lexeme == "while")
            {
                this->post_advance();
                if (this->get_token().M_type != token_type::TOKEN_LEFT_PAREN)
                {
                    this->handle_eof();
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '(' before", this->get_token().M_lexeme.wrap("'")});
                    return nullptr;
                }
                else
                    this->post_advance();
                if (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN)
                {
                    condition = this->parse_operators();
                    if (!condition)
                        return nullptr;
                    if (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN)
                    {
                        this->handle_eof();
                        horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ')', but got", this->get_token().M_lexeme.wrap("'")});
                        return nullptr;
                    }
                    this->post_advance();
                }
                else
                {
                    this->handle_eof();
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected an expression before", this->get_token().M_lexeme.wrap("'")});
                    return nullptr;
                }
                return new ast_do_while_loop_node(std::move(block), std::move(condition));
            }
            else
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected 'while', but got", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
        }

        horizon_deps::sptr<ast_node> parser::parse_while_loop()
        {
            if (this->get_token().M_lexeme != "while")
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected 'while', but got", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            horizon_deps::sptr<ast_node> condition = nullptr;
            horizon_deps::sptr<ast_node> block = nullptr;

            this->post_advance();
            if (this->get_token().M_type != token_type::TOKEN_LEFT_PAREN)
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '(' before", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            else
                this->post_advance();
            if (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN)
            {
                condition = this->parse_operators();
                if (!condition)
                    return nullptr;
                if (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN)
                {
                    this->handle_eof();
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ')', but got", this->get_token().M_lexeme.wrap("'")});
                    return nullptr;
                }
                this->post_advance();
            }
            else
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected an expression before", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            if (this->get_token().M_type != token_type::TOKEN_LEFT_BRACE)
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '{', but got", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            block = this->parse_block();
            if (!block)
                return nullptr;
            return new ast_while_loop_node(std::move(condition), std::move(block));
        }

        horizon_deps::sptr<ast_node> parser::parse_for_loop()
        {
            if (this->get_token().M_lexeme != "for")
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected 'for', but got", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            horizon_deps::sptr<ast_node> variable_decl = nullptr;
            horizon_deps::sptr<ast_node> condition = nullptr;
            horizon_deps::sptr<ast_node> step = nullptr;
            horizon_deps::sptr<ast_node> block = nullptr;

            this->post_advance();
            if (this->get_token().M_type != token_type::TOKEN_LEFT_PAREN)
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '(' before", this->get_token().M_lexeme.wrap("'")});
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
                    this->handle_eof();
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ')', but got", this->get_token().M_lexeme.wrap("'")});
                    return nullptr;
                }
            }
            this->post_advance();
            if (this->get_token().M_type != token_type::TOKEN_LEFT_BRACE)
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '{', but got", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            block = this->parse_block();
            if (!block)
                return nullptr;
            return new ast_for_loop_node(std::move(variable_decl), std::move(condition), std::move(step), std::move(block));
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
                        this->handle_eof();
                        horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '(' before", this->get_token().M_lexeme.wrap("'")});
                        return nullptr;
                    }
                    if_condition_block.raw_first() = new horizon_deps::sptr<ast_node>(this->parse_operators());
                    if (!(*if_condition_block.raw_first()))
                        return nullptr;
                    if_condition_block.raw_second() = new horizon_deps::sptr<ast_node>(this->parse_block());
                    if (!(*if_condition_block.raw_second()))
                        return nullptr;
                }

                if (this->get_token().M_lexeme == "elif")
                {
                    while (this->get_token().M_lexeme == "elif")
                    {
                        horizon_deps::pair<horizon_deps::sptr<ast_node>> temp = {nullptr, nullptr};
                        this->post_advance();
                        if (this->get_token().M_type != token_type::TOKEN_LEFT_PAREN)
                        {
                            this->handle_eof();
                            horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '(' before", this->get_token().M_lexeme.wrap("'")});
                            return nullptr;
                        }
                        temp.raw_first() = new horizon_deps::sptr<ast_node>(this->parse_operators());
                        if (!(*temp.raw_first()))
                            return nullptr;
                        temp.raw_second() = new horizon_deps::sptr<ast_node>(this->parse_block());
                        if (!(*temp.raw_second()))
                            return nullptr;
                        elif_condition_block.add(std::move(temp));
                    }
                    elif_condition_block.shrink_to_fit();
                }

                if (this->get_token().M_lexeme == "else")
                {
                    this->post_advance();
                    else_block = this->parse_block();
                    if (!else_block)
                        return nullptr;
                }

                return new ast_if_elif_else_node(std::move(if_condition_block), std::move(elif_condition_block), std::move(else_block));
            }
            else
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {this->get_token().M_lexeme.wrap("'"), "without a prior 'if'"});
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
                        horizon_deps::sptr<ast_node> temp = this->parse_block();
                        if (!temp)
                            return nullptr;
                        nodes.add(std::move(temp));
                    }
                    else
                    {
                        bool is_var_decl = false;
                        std::size_t temp_curr_parser = this->M_current_parser;
                        while (this->get_token().M_type != token_type::TOKEN_SEMICOLON && !this->has_reached_end())
                        {
                            if (this->post_advance().M_type == token_type::TOKEN_COLON)
                            {
                                is_var_decl = true;
                                break;
                            }
                        }
                        this->M_current_parser = temp_curr_parser;
                        if (this->get_token().M_type == token_type::TOKEN_KEYWORD &&
                            this->get_token().M_lexeme != "let" &&
                            this->get_token().M_lexeme != "const" &&
                            this->get_token().M_lexeme != "ref" &&
                            this->get_token().M_lexeme != "static")
                        {
                            horizon_deps::sptr<ast_node> temp = nullptr;
                            if (this->get_token().M_lexeme == "if" ||
                                this->get_token().M_lexeme == "elif" ||
                                this->get_token().M_lexeme == "else")
                                temp = this->parse_if_elif_else();
                            else if (this->get_token().M_lexeme == "for")
                                temp = this->parse_for_loop();
                            else if (this->get_token().M_lexeme == "while")
                                temp = this->parse_while_loop();
                            else if (this->get_token().M_lexeme == "do")
                                temp = this->parse_do_while_loop();
                            else if (this->get_token().M_lexeme == "func")
                                temp = this->parse_function();
                            else if (this->get_token().M_lexeme == "return" ||
                                     this->get_token().M_lexeme == "break" ||
                                     this->get_token().M_lexeme == "continue")
                                temp = this->parse_jump_statements();
                            if (!temp)
                                return nullptr;
                            nodes.add(std::move(temp));
                        }
                        else if (is_var_decl)
                        {
                            horizon_deps::sptr<ast_node> x1 = this->parse_variable_decl();
                            if (!x1)
                                return nullptr;
                            if (!this->handle_semicolon())
                                return nullptr;
                            nodes.add(std::move(x1));
                        }
                        else
                        {
                            horizon_deps::sptr<ast_node> x1 = this->parse_operators();
                            if (!x1)
                                return nullptr;
                            if (!this->handle_semicolon())
                                return nullptr;
                            nodes.add(std::move(x1));
                        }
                    }
                }
                if (this->get_token().M_type != token_type::TOKEN_RIGHT_BRACE)
                {
                    this->handle_eof();
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected '}', but got", this->get_token().M_lexeme.wrap("'")});
                    return nullptr;
                }
                else
                    this->post_advance();
                nodes.shrink_to_fit();
                return new ast_block_node(std::move(nodes));
            }
            return nullptr;
        }

        horizon_deps::sptr<ast_node> parser::parse_variable_decl()
        {
            horizon_deps::sptr<ast_node> type_ = this->parse_data_type();
            if (!type_)
                return nullptr;
            horizon_deps::vector<horizon_deps::pair<token, horizon_deps::sptr<ast_node>>> vec(5);
            if (this->get_token().M_type != token_type::TOKEN_COLON)
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ':' after the data type, but got", this->get_token().M_lexeme.wrap("'")});
                return nullptr;
            }
            this->post_advance();
            while (this->get_token().M_type != token_type::TOKEN_SEMICOLON && !this->has_reached_end())
            {
                horizon_deps::pair<token, horizon_deps::sptr<ast_node>> pair;
                if (this->get_token().M_type == token_type::TOKEN_PRIMARY_TYPE || this->get_token().M_type == token_type::TOKEN_KEYWORD)
                {
                    this->handle_eof();
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"reserved word", this->get_token().M_lexeme.wrap("'"), "cannot be used as an identifier"});
                    return nullptr;
                }
                else if (this->get_token().M_type == token_type::TOKEN_IDENTIFIER)
                {
                    pair.raw_first() = new token(std::move(this->get_token()));
                }
                else
                {
                    this->handle_eof();
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected an identifier, but got", this->get_token().M_lexeme.wrap("'")});
                    return nullptr;
                }
                this->post_advance();
                if (this->get_token().M_type == token_type::TOKEN_ASSIGN)
                {
                    this->post_advance();
                    pair.raw_second() = new horizon_deps::sptr<ast_node>(this->parse_operators());
                    if (!(*pair.raw_second()))
                        return nullptr;
                    if (this->get_token().M_type == token_type::TOKEN_COMMA)
                        this->post_advance();
                    else if (this->get_token().M_type != token_type::TOKEN_SEMICOLON)
                    {
                        this->handle_eof();
                        horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"unexpected token", this->get_token().M_lexeme.wrap("'"), "expected ';'"});
                        return nullptr;
                    }
                }
                else if (this->get_token().M_type == token_type::TOKEN_COMMA)
                {
                    this->post_advance();
                    pair.raw_second() = nullptr;
                }
                else if (this->get_token().M_type != token_type::TOKEN_SEMICOLON)
                {
                    this->handle_eof();
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"unexpected token", this->get_token().M_lexeme.wrap("'"), "expected ';'"});
                    return nullptr;
                }
                vec.add(std::move(pair));
            }
            vec.shrink_to_fit();
            return new ast_variable_declaration_node(std::move(type_), std::move(vec));
        }

        horizon_deps::sptr<ast_node> parser::parse_operators()
        {
            return this->parse_assignment_operator();
        }

        horizon_deps::sptr<ast_node> parser::parse_assignment_operator()
        {
            horizon_deps::sptr<ast_node> left = this->parse_ternary_operator();
            if (!left)
                return nullptr;
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
                token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_ternary_operator();
                if (!right)
                    return nullptr;
                left = new ast_binary_operation_node(std::move(left), std::move(operator_token), std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_ternary_operator()
        {
            // a < b ? true : false
            horizon_deps::sptr<ast_node> condition;
            horizon_deps::sptr<ast_node> val_if_true;
            horizon_deps::sptr<ast_node> val_if_false;
            bool is_if_used = false;
            condition = this->parse_logical_or(); // if `if` is used, then it is value_if_true
            if (!condition)
                return nullptr;
            if (this->get_token().M_type != token_type::TOKEN_QUESTION)
            {
                if (this->get_token().M_lexeme == "if")
                    is_if_used = true;
                else
                    // this was not a ternary operator, as there is no ? token
                    // Hence, we will return the condition or now it is an expression too
                    return condition;
            }
            this->post_advance();
            val_if_true = this->parse_logical_or(); // if `if` is used, then it is condition
            if (!val_if_true)
                return nullptr;
            if (this->get_token().M_type != token_type::TOKEN_COLON)
            {
                if (this->get_token().M_lexeme != "else")
                {
                    if (is_if_used)
                    {
                        this->handle_eof();
                        horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected 'else', but got", this->get_token().M_lexeme.wrap("'")});
                        return nullptr;
                    }
                    else
                    {
                        this->handle_eof();
                        horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ':', but got", this->get_token().M_lexeme.wrap("'")});
                        return nullptr;
                    }
                }
            }
            this->post_advance();
            val_if_false = this->parse_logical_or();
            if (!val_if_false)
                return nullptr;
            if (is_if_used)
                return new ast_ternary_operator_node(std::move(val_if_true), std::move(condition), std::move(val_if_false));
            return new ast_ternary_operator_node(std::move(condition), std::move(val_if_true), std::move(val_if_false));
        }

        horizon_deps::sptr<ast_node> parser::parse_logical_or()
        {
            horizon_deps::sptr<ast_node> left = this->parse_logical_and();
            if (!left)
                return nullptr;
            while (this->get_token().M_type == token_type::TOKEN_LOGICAL_OR)
            {
                token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_logical_and();
                if (!right)
                    return nullptr;
                left = new ast_binary_operation_node(std::move(left), std::move(operator_token), std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_logical_and()
        {
            horizon_deps::sptr<ast_node> left = this->parse_bitwise_or();
            if (!left)
                return nullptr;
            while (this->get_token().M_type == token_type::TOKEN_LOGICAL_AND)
            {
                token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_bitwise_or();
                if (!right)
                    return nullptr;
                left = new ast_binary_operation_node(std::move(left), std::move(operator_token), std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_bitwise_or()
        {
            horizon_deps::sptr<ast_node> left = this->parse_bitwise_xor();
            if (!left)
                return nullptr;
            while (this->get_token().M_type == token_type::TOKEN_BITWISE_OR)
            {
                token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_bitwise_xor();
                if (!right)
                    return nullptr;
                left = new ast_binary_operation_node(std::move(left), std::move(operator_token), std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_bitwise_xor()
        {
            horizon_deps::sptr<ast_node> left = this->parse_bitwise_and();
            if (!left)
                return nullptr;
            while (this->get_token().M_type == token_type::TOKEN_BITWISE_XOR)
            {
                token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_bitwise_and();
                if (!right)
                    return nullptr;
                left = new ast_binary_operation_node(std::move(left), std::move(operator_token), std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_bitwise_and()
        {
            horizon_deps::sptr<ast_node> left = this->parse_equality_operator();
            if (!left)
                return nullptr;
            while (this->get_token().M_type == token_type::TOKEN_BITWISE_AND)
            {
                token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_equality_operator();
                if (!right)
                    return nullptr;
                left = new ast_binary_operation_node(std::move(left), std::move(operator_token), std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_equality_operator()
        {
            horizon_deps::sptr<ast_node> left = this->parse_relational_operator();
            if (!left)
                return nullptr;
            while (this->get_token().M_type == token_type::TOKEN_RELATIONAL_EQUAL_TO ||
                   this->get_token().M_type == token_type::TOKEN_RELATIONAL_NOT_EQUAL_TO)
            {
                token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_relational_operator();
                if (!right)
                    return nullptr;
                left = new ast_binary_operation_node(std::move(left), std::move(operator_token), std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_relational_operator()
        {
            horizon_deps::sptr<ast_node> left = this->parse_bitwise_shift();
            if (!left)
                return nullptr;
            while (this->get_token().M_type == token_type::TOKEN_RELATIONAL_GREATER_THAN ||
                   this->get_token().M_type == token_type::TOKEN_RELATIONAL_LESS_THAN ||
                   this->get_token().M_type == token_type::TOKEN_RELATIONAL_GREATER_THAN_OR_EQUAL_TO ||
                   this->get_token().M_type == token_type::TOKEN_RELATIONAL_LESS_THAN_OR_EQUAL_TO)
            {
                token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_bitwise_shift();
                if (!right)
                    return nullptr;
                left = new ast_binary_operation_node(std::move(left), std::move(operator_token), std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_bitwise_shift()
        {
            horizon_deps::sptr<ast_node> left = this->parse_expr();
            if (!left)
                return nullptr;
            while (this->get_token().M_type == token_type::TOKEN_BITWISE_LEFT_SHIFT ||
                   this->get_token().M_type == token_type::TOKEN_BITWISE_RIGHT_SHIFT)
            {
                token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_expr();
                if (!right)
                    return nullptr;
                left = new ast_binary_operation_node(std::move(left), std::move(operator_token), std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_expr()
        {
            horizon_deps::sptr<ast_node> left = this->parse_term();
            if (!left)
                return nullptr;
            while (this->get_token().M_type == token_type::TOKEN_ARITHMETIC_ADD ||
                   this->get_token().M_type == token_type::TOKEN_ARITHMETIC_SUBSTRACT)
            {
                token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_term();
                if (!right)
                    return nullptr;
                left = new ast_binary_operation_node(std::move(left), std::move(operator_token), std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_term()
        {
            horizon_deps::sptr<ast_node> left = this->parse_exponent();
            if (!left)
                return nullptr;
            while (this->get_token().M_type == token_type::TOKEN_ARITHMETIC_MULTIPLY ||
                   this->get_token().M_type == token_type::TOKEN_ARITHMETIC_DIVIDE ||
                   this->get_token().M_type == token_type::TOKEN_ARITHMETIC_MODULUS)
            {
                token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_exponent();
                if (!right)
                    return nullptr;
                left = new ast_binary_operation_node(std::move(left), std::move(operator_token), std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_exponent()
        {
            horizon_deps::sptr<ast_node> left = this->parse_unary_operators();
            if (!left)
                return nullptr;
            while (this->get_token().M_type == token_type::TOKEN_ARITHMETIC_POWER)
            {
                token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_unary_operators();
                if (!right)
                    return nullptr;
                left = new ast_binary_operation_node(std::move(left), std::move(operator_token), std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_unary_operators()
        {
            token_type current_type = this->get_token().M_type;
            if (current_type == token_type::TOKEN_INCREMENT ||
                current_type == token_type::TOKEN_DECREMENT ||
                current_type == token_type::TOKEN_ARITHMETIC_ADD ||
                current_type == token_type::TOKEN_ARITHMETIC_SUBSTRACT ||
                current_type == token_type::TOKEN_LOGICAL_NOT ||
                current_type == token_type::TOKEN_BITWISE_NOT)
            {
                token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> operand = this->parse_unary_operators();
                if (!operand)
                    return nullptr;
                return new ast_unary_operation_node(std::move(operand), std::move(operator_token), true);
            }
            else
            {
                horizon_deps::sptr<ast_node> left = this->parse_member_access();
                if (!left)
                    return nullptr;
                while (this->get_token().M_type == token_type::TOKEN_INCREMENT ||
                       this->get_token().M_type == token_type::TOKEN_DECREMENT)
                {
                    token &operator_token = this->post_advance();
                    left = new ast_unary_operation_node(std::move(left), std::move(operator_token), false);
                }
                return left;
            }
        }

        horizon_deps::sptr<ast_node> parser::parse_member_access()
        {
            horizon_deps::sptr<ast_node> left = this->parse_identifier();
            if (!left)
                return nullptr;
            while (this->get_token().M_type == token_type::TOKEN_DOT ||
                   this->get_token().M_type == token_type::TOKEN_MEMEBER_ACCESS)
            {
                token &operator_token = this->post_advance();
                horizon_deps::sptr<ast_node> right = this->parse_identifier();
                if (!right)
                    return nullptr;
                left = new ast_binary_operation_node(std::move(left), std::move(operator_token), std::move(right));
            }
            return left;
        }

        horizon_deps::sptr<ast_node> parser::parse_identifier()
        {
            if (this->get_token().M_type == token_type::TOKEN_IDENTIFIER)
            {
                token &identifier = this->post_advance();
                if (this->get_token().M_type == token_type::TOKEN_LEFT_PAREN)
                {
                    this->post_advance();
                    horizon_deps::vector<horizon_deps::sptr<ast_node>> vec(5);
                    while (this->get_token().M_type != token_type::TOKEN_RIGHT_PAREN && !this->has_reached_end())
                    {
                        horizon_deps::sptr<ast_node> temp = this->parse_operators();
                        if (!temp)
                            return nullptr;
                        vec.add(std::move(temp));
                        if (this->get_token().M_type == token_type::TOKEN_COMMA)
                            this->post_advance();
                        else if (this->get_token().M_type == token_type::TOKEN_RIGHT_PAREN)
                            break;
                        else
                        {
                            this->handle_eof();
                            horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"unexpected token", this->get_token().M_lexeme.wrap("'"), "expected ')'"});
                            return nullptr;
                        }
                    }
                    if (this->get_token().M_type == token_type::TOKEN_RIGHT_PAREN)
                        this->post_advance();
                    else
                    {
                        this->handle_eof();
                        horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ')', but got", this->get_token().M_lexeme.wrap("'")});
                        return nullptr;
                    }
                    vec.shrink_to_fit();
                    return new ast_function_call_node(std::move(identifier), std::move(vec));
                }
                else
                    return new ast_operand_node(std::move(identifier.M_lexeme));
            }
            else if (this->get_token().M_type == token_type::TOKEN_KEYWORD)
            {
                if (this->get_token().M_lexeme == "true")
                {
                    this->post_advance();
                    return new ast_operand_node(true);
                }
                else if (this->get_token().M_lexeme == "false")
                {
                    this->post_advance();
                    return new ast_operand_node(false);
                }
                else if (this->get_token().M_lexeme == "null")
                {
                    this->post_advance();
                    return new ast_operand_node<void *>(nullptr);
                }
            }
            return this->parse_brackets();
        }

        horizon_deps::sptr<ast_node> parser::parse_brackets()
        {
            if (this->get_token().M_type == token_type::TOKEN_LEFT_PAREN)
            {
                this->post_advance();
                horizon_deps::sptr<ast_node> x = this->parse_operators();
                if (!x)
                    return nullptr;
                if (this->get_token().M_type == token_type::TOKEN_RIGHT_PAREN)
                    this->post_advance();
                else
                {
                    this->handle_eof();
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"expected ')', but got", this->get_token().M_lexeme.wrap("'")});
                    return nullptr;
                }
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
                const char *ptr = this->get_token().M_lexeme.c_str();
                this->post_advance().M_lexeme.raw() = nullptr;
                return new ast_operand_node(ptr);
            }
            else if (this->get_token().M_type == token_type::TOKEN_CHAR_LITERAL)
            {
                return new ast_operand_node(*this->post_advance().M_lexeme.c_str());
            }
            else
            {
                this->handle_eof();
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->get_token(), {"unexpected token", this->get_token().M_lexeme.wrap("'")});
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
            this->M_ast = this->parse_program();
            if (!this->M_ast)
                return false;
            this->M_tokens.erase();
            this->M_ast->print();
            return true;
        }
    }
}