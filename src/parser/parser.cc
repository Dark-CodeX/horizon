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

        bool parser::parse_function()
        {
            this->M_current_parser++;
            function_declaration fd;
            if (this->M_tokens[this->M_current_parser].M_type != token_type::TOKEN_IDENTIFIER)
            {
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->M_tokens[this->M_current_parser], {"expected an identifier, but got", this->M_tokens[this->M_current_parser].M_lexeme});
                return false;
            }
            fd.M_identifier = this->M_tokens[this->M_current_parser++].M_lexeme;
            if (this->M_tokens[this->M_current_parser].M_type != token_type::TOKEN_LEFT_PAREN)
            {
                horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->M_tokens[this->M_current_parser], {"expected '(', but got", this->M_tokens[this->M_current_parser].M_lexeme});
                return false;
            }
            this->M_current_parser++;
            while (this->M_tokens[this->M_current_parser].M_type != token_type::TOKEN_RIGHT_PAREN && !this->has_reached_end())
            {
                horizon_deps::vector<horizon_deps::string> temp_type_vec(10);
                while (this->M_tokens[this->M_current_parser].M_type != token_type::TOKEN_COLON && !this->has_reached_end())
                {
                    if (this->M_tokens[this->M_current_parser].M_type != token_type::TOKEN_IDENTIFIER && this->M_tokens[this->M_current_parser].M_type != token_type::TOKEN_PRIMARY_TYPE)
                    {
                        horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->M_tokens[this->M_current_parser], {"expected an identifier, but got", this->M_tokens[this->M_current_parser].M_lexeme});
                        return false;
                    }
                    temp_type_vec.add(this->M_tokens[this->M_current_parser++].M_lexeme);
                }
                if (this->M_tokens[this->M_current_parser].M_type != token_type::TOKEN_COLON)
                {
                    horizon_errors::errors::parser_draw_error(horizon_errors::error_code::HORIZON_SYNTAX_ERROR, this->M_file, this->M_tokens[this->M_current_parser], {"expected ':', but got", this->M_tokens[this->M_current_parser].M_lexeme});
                    return false;
                }
                this->M_current_parser++;
                if (this->M_tokens[this->M_current_parser].M_type != token_type::TOKEN_IDENTIFIER)
                {
                    return false;
                }
                const std::pair<std::map<horizon_deps::string, horizon_deps::vector<horizon_deps::string>>::iterator, bool> &iter = fd.M_parameter.M_container.insert({this->M_tokens[this->M_current_parser].M_lexeme, std::move(temp_type_vec)});
                if (!iter.second)
                {
                    return false;
                }
                this->M_current_parser++;
                if (this->M_tokens[this->M_current_parser].M_type == token_type::TOKEN_COMMA)
                    this->M_current_parser++;
            }

            if (fd.M_parameter.M_container.empty())
            {
                fd.M_parameter.M_container[nullptr] = {"void"};
            }

            if (this->M_tokens[++this->M_current_parser].M_type == token_type::TOKEN_COLON)
            {
                this->M_current_parser++;
                horizon_deps::vector<horizon_deps::string> temp_ret_vec(10);
                while (this->M_tokens[this->M_current_parser].M_type != token_type::TOKEN_LEFT_BRACE && !this->has_reached_end())
                {
                    if (this->M_tokens[this->M_current_parser].M_type != token_type::TOKEN_IDENTIFIER && this->M_tokens[this->M_current_parser].M_type != token_type::TOKEN_PRIMARY_TYPE)
                        return false;
                    temp_ret_vec.add(this->M_tokens[this->M_current_parser++].M_lexeme);
                }
                fd.M_return_type = std::move(temp_ret_vec);
            }
            else
            {
                fd.M_return_type = {"ctor"};
            }

            {
                std::printf("NAME: '%s'  RETURN: ", fd.M_identifier.c_str());
                for (std::size_t i = 0; i < fd.M_return_type.length(); i++)
                {
                    std::printf("'%s' ", fd.M_return_type[i].c_str());
                }
                std::printf("PARAMETER: ");
                for (const auto &i : fd.M_parameter.M_container)
                {
                    std::printf("'%s' ", i.first.c_str());
                    for (std::size_t j = 0; j < i.second.length(); j++)
                    {
                        std::printf("'%s' ", i.second[j].c_str());
                    }
                }
                std::printf("\n");
            }

            return true;
        }

        bool parser::read_tokens()
        {
            while (!this->has_reached_end())
            {
                const token &current_tok = this->M_tokens[this->M_current_parser];
                if (current_tok.M_lexeme == "func")
                {
                    if (!this->parse_function())
                        return false;
                }
                else
                {
                    this->M_current_parser++;
                }
            }
            this->M_tokens.erase();
            return true;
        }

        parser::parser(horizon_deps::vector<token> &&movable_vec, horizon_misc::HR_FILE *file)
        {
            this->M_tokens = std::move(movable_vec);
            this->M_file = file;
            this->M_current_parser = 0;
        }

        bool parser::init_parsing()
        {
            return read_tokens();
        }

        void parser::debug_print() const
        {
        }
    }
}