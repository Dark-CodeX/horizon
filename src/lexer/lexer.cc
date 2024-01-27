/**
 * @file lexer.cc
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#include "./lexer.hh"

namespace horizon
{
    namespace horizon_lexer
    {
        bool lexer::scan_tokens()
        {
            while (!this->has_reached_eof())
            {
                this->M_start_lexer = this->M_current_lexer;
                this->load_char();
                switch (this->M_ch)
                {
                case '+':
                    this->pre_advance_lexer();
                    if (this->M_ch == '+')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_INCREMENT);
                    }
                    else if (this->M_ch == '=')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_ASSIGN_ADD);
                    }
                    else
                    {
                        this->append_token(token_type::TOKEN_ARITHMETIC_ADD);
                    }
                    break;

                case '-':
                    this->pre_advance_lexer();
                    if (this->M_ch == '-')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_DECREMENT);
                    }
                    else if (this->M_ch == '=')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_ASSIGN_SUBSTRACT);
                    }
                    else
                    {
                        this->append_token(token_type::TOKEN_ARITHMETIC_SUBSTRACT);
                    }
                    break;

                case '*':
                    this->pre_advance_lexer();
                    if (this->M_ch == '=')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_ASSIGN_MULTIPLY);
                    }
                    else if (this->M_ch == '*')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_ARITHMETIC_POWER);
                    }
                    else
                    {
                        this->append_token(token_type::TOKEN_ARITHMETIC_MULTIPLY);
                    }
                    break;

                case '/':
                    this->pre_advance_lexer();
                    if (this->M_ch == '=')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_ASSIGN_DIVIDE);
                    }
                    else
                    {
                        this->append_token(token_type::TOKEN_ARITHMETIC_DIVIDE);
                    }
                    break;

                case '%':
                    this->pre_advance_lexer();
                    if (this->M_ch == '=')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_ASSIGN_MODULUS);
                    }
                    else
                    {
                        this->append_token(token_type::TOKEN_ARITHMETIC_MODULUS);
                    }
                    break;

                case '^':
                    this->pre_advance_lexer();
                    if (this->M_ch == '=')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_ASSIGN_BITWISE_XOR);
                    }
                    else
                    {
                        this->append_token(token_type::TOKEN_BITWISE_XOR);
                    }
                    break;

                case '~':
                    this->pre_advance_lexer();
                    this->append_token(token_type::TOKEN_BITWISE_NOT);
                    break;

                case '&':
                    this->pre_advance_lexer();
                    if (this->M_ch == '&')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_LOGICAL_AND);
                    }
                    else if (this->M_ch == '=')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_ASSIGN_BITWISE_AND);
                    }
                    else
                    {
                        this->append_token(token_type::TOKEN_BITWISE_AND);
                    }
                    break;

                case '|':
                    this->pre_advance_lexer();
                    if (this->M_ch == '|')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_LOGICAL_OR);
                    }
                    else if (this->M_ch == '=')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_ASSIGN_BITWISE_OR);
                    }
                    else
                    {
                        this->append_token(token_type::TOKEN_BITWISE_OR);
                    }
                    break;

                case '!':
                    this->pre_advance_lexer();
                    if (this->M_ch == '=')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_RELATIONAL_NOT_EQUAL_TO);
                    }
                    else
                    {
                        this->append_token(token_type::TOKEN_LOGICAL_NOT);
                    }
                    break;

                case '=':
                    this->pre_advance_lexer();
                    if (this->M_ch == '=')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_RELATIONAL_EQUAL_TO);
                    }
                    else
                    {
                        this->append_token(token_type::TOKEN_ASSIGN);
                    }
                    break;

                case '<':
                    this->pre_advance_lexer();
                    if (this->M_ch == '<')
                    {
                        this->pre_advance_lexer();
                        if (this->M_ch == '=')
                        {
                            this->pre_advance_lexer();
                            this->append_token(token_type::TOKEN_ASSIGN_LEFT_SHIFT);
                        }
                        else
                        {
                            this->append_token(token_type::TOKEN_BITWISE_LEFT_SHIFT);
                        }
                    }
                    else if (this->M_ch == '=')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_RELATIONAL_LESS_THAN_OR_EQUAL_TO);
                    }
                    else
                    {
                        this->append_token(token_type::TOKEN_RELATIONAL_LESS_THAN);
                    }
                    break;

                case '>':
                    this->pre_advance_lexer();
                    if (this->M_ch == '>')
                    {
                        this->pre_advance_lexer();
                        if (this->M_ch == '=')
                        {
                            this->pre_advance_lexer();
                            this->append_token(token_type::TOKEN_ASSIGN_RIGHT_SHIFT);
                        }
                        else
                        {
                            this->append_token(token_type::TOKEN_BITWISE_RIGHT_SHIFT);
                        }
                    }
                    else if (this->M_ch == '=')
                    {
                        this->pre_advance_lexer();
                        this->append_token(token_type::TOKEN_RELATIONAL_GREATER_THAN_OR_EQUAL_TO);
                    }
                    else
                    {
                        this->append_token(token_type::TOKEN_RELATIONAL_GREATER_THAN);
                    }
                    break;

                case '?':
                    this->pre_advance_lexer();
                    this->append_token(token_type::TOKEN_QUESTION);
                    break;

                case '\n':
                case '\r':
                case '\t':
                case ' ':
                    this->pre_advance_lexer(); // here space, newline, tab and carriage is ignored
                    break;

                case '(':
                    this->pre_advance_lexer();
                    this->append_token(token_type::TOKEN_LEFT_PAREN);
                    break;
                case ')':
                    this->pre_advance_lexer();
                    this->append_token(token_type::TOKEN_RIGHT_PAREN);
                    break;
                case '{':
                    this->pre_advance_lexer();
                    this->append_token(token_type::TOKEN_LEFT_BRACE);
                    break;
                case '}':
                    this->pre_advance_lexer();
                    this->append_token(token_type::TOKEN_RIGHT_BRACE);
                    break;
                case '[':
                    this->pre_advance_lexer();
                    this->append_token(token_type::TOKEN_LEFT_BRACKET);
                    break;
                case ']':
                    this->pre_advance_lexer();
                    this->append_token(token_type::TOKEN_RIGHT_BRACKET);
                    break;

                case ';':
                    this->pre_advance_lexer();
                    this->append_token(token_type::TOKEN_SEMICOLON);
                    break;
                case ':':
                    this->pre_advance_lexer();
                    this->append_token(token_type::TOKEN_COLON);
                    break;
                case ',':
                    this->pre_advance_lexer();
                    this->append_token(token_type::TOKEN_COMMA);
                    break;

                default:
                    if (this->M_ch == '`')
                    {
                        horizon_errors::error_code ecode = this->handle_comments();
                        if (ecode == horizon_errors::error_code::HORIZON_MISSING_TERMINATING_CHAR)
                        {
                            horizon_errors::errors::lexer_draw_error(ecode, this->M_file, this->M_line, this->M_start_lexer, this->M_current_lexer, {"unterminated comment", "expected `"});
                            return false;
                        }
                    }
                    else if (this->M_ch == '"')
                    {
                        horizon_errors::error_code ecode = this->handle_string();
                        if (ecode == horizon_errors::error_code::HORIZON_INVALID_ESCAPE_SEQUENCE)
                        {
                            horizon_errors::errors::lexer_draw_error(ecode, this->M_file, this->M_line, this->M_start_lexer, this->M_current_lexer, {"invalid escape sequence", horizon_deps::string("\\") + this->M_invalid_ec});
                            return false;
                        }
                        else if (ecode == horizon_errors::error_code::HORIZON_MISSING_TERMINATING_CHAR)
                        {
                            horizon_errors::errors::lexer_draw_error(ecode, this->M_file, this->M_line, this->M_start_lexer, this->M_current_lexer, {"missing terminating character", "expected", "\""});
                            return false;
                        }
                    }
                    else if (this->M_ch == '\'')
                    {
                        horizon_errors::error_code ecode = this->handle_char();
                        if (ecode == horizon_errors::error_code::HORIZON_INVALID_ESCAPE_SEQUENCE)
                        {
                            horizon_errors::errors::lexer_draw_error(ecode, this->M_file, this->M_line, this->M_start_lexer, this->M_current_lexer, {"invalid escape sequence", horizon_deps::string("\\") + this->M_invalid_ec});
                            return false;
                        }
                        else if (ecode == horizon_errors::error_code::HORIZON_MISSING_TERMINATING_CHAR)
                        {
                            horizon_errors::errors::lexer_draw_error(ecode, this->M_file, this->M_line, this->M_start_lexer, this->M_current_lexer, {"missing terminating character", "expected", "'"});
                            return false;
                        }
                        else if (ecode == horizon_errors::error_code::HORIZON_LESS_BYTES)
                        {
                            horizon_errors::errors::lexer_draw_error(ecode, this->M_file, this->M_line, this->M_start_lexer, this->M_current_lexer, {"empty character constant"});
                            return false;
                        }
                        else if (ecode == horizon_errors::error_code::HORIZON_MORE_BYTES)
                        {
                            horizon_errors::errors::lexer_draw_error(horizon_errors::error_code::HORIZON_MORE_BYTES, this->M_file, this->M_line, this->M_start_lexer, this->M_current_lexer, {"multi-character character constant"});
                        }
                    }
                    else if (std::isdigit(this->M_ch))
                    {
                        this->handle_integer_decimal();
                    }
                    else if (this->M_ch == '.')
                    {
                        // if not decimal number, then it must be dot like `this.xyz`
                        if (!this->handle_integer_decimal())
                        {
                            this->append_token(token_type::TOKEN_DOT);
                        }
                    }
                    else if (std::isalpha(this->M_ch) || this->M_ch == '_' || this->M_ch == '$')
                    {
                        this->handle_identifier();
                    }
                    else
                    {
                        horizon_errors::errors::lexer_draw_error(horizon_errors::error_code::HORIZON_UNKNOWN_TOKEN, this->M_file, this->M_line, this->M_start_lexer, this->M_current_lexer, {"unrecognized token", this->M_ch});
                        return false;
                    }
                    break;
                }
            }
            return true;
        }

        void lexer::append_token(const token_type &type)
        {
            horizon_deps::string temp(this->M_file->M_content.substr(this->M_start_lexer, this->M_current_lexer - this->M_start_lexer));
            if (type == token_type::TOKEN_IDENTIFIER)
            {
                if (is_keyword(temp.c_str()))
                    this->M_tokens.add(token{token_type::TOKEN_KEYWORD, std::move(temp), this->M_start_lexer, this->M_current_lexer});
                else if (is_primary_data_type(temp.c_str()))
                    this->M_tokens.add(token{token_type::TOKEN_PRIMARY_TYPE, std::move(temp), this->M_start_lexer, this->M_current_lexer});
                else
                    this->M_tokens.add(token{type, std::move(temp), this->M_start_lexer, this->M_current_lexer});
            }
            else
                this->M_tokens.add(token{type, std::move(temp), this->M_start_lexer, this->M_current_lexer});
        }

        bool lexer::has_reached_eof() const
        {
            return this->M_current_lexer >= this->M_file->M_content.length();
        }

        void lexer::post_advance_lexer()
        {
            this->M_ch = this->M_file->M_content[this->M_current_lexer++];
            if (this->M_ch == '\n')
            {
                this->M_line++;
            }
        }

        void lexer::pre_advance_lexer()
        {
            if (this->M_ch == '\n')
            {
                this->M_line++;
            }
            this->M_ch = this->M_file->M_content[++this->M_current_lexer];
        }

        void lexer::load_char()
        {
            char temp_ch = this->M_ch;
            this->M_ch = this->M_file->M_content[this->M_current_lexer];
            if (temp_ch != this->M_ch)
            {
                if (this->M_ch == '\n')
                {
                    this->M_line++;
                }
            }
        }

        bool lexer::handle_integer_decimal()
        {
            if (this->M_ch == '.')
            {
                this->pre_advance_lexer();
                if (!std::isdigit(this->M_ch))
                    return false;
                else
                {
                    while (!this->has_reached_eof())
                    {
                        this->pre_advance_lexer();
                        if (!std::isdigit(this->M_ch))
                            break;
                    }
                    this->append_token(token_type::TOKEN_DECIMAL_LITERAL);
                    return true;
                }
            }
            else
            {
                bool is_decimal = false;
                while (!this->has_reached_eof())
                {
                    this->pre_advance_lexer();
                    if (!std::isdigit(this->M_ch))
                        break;
                }
                if (this->M_ch == '.')
                {
                    is_decimal = true;
                    this->pre_advance_lexer();
                    if (std::isdigit(this->M_ch))
                    {
                        while (!this->has_reached_eof())
                        {
                            this->pre_advance_lexer();
                            if (!std::isdigit(this->M_ch))
                                break;
                        }
                    }
                }
                this->append_token((is_decimal ? token_type::TOKEN_DECIMAL_LITERAL : token_type::TOKEN_INTEGER_LITERAL));
                return true;
            }
        }

        void lexer::handle_identifier()
        {
            while (!this->has_reached_eof())
            {
                if (!std::isalnum(this->M_ch) && this->M_ch != '_' && this->M_ch != '$')
                    break;
                this->pre_advance_lexer();
            }
            this->append_token(token_type::TOKEN_IDENTIFIER);
        }

        horizon_errors::error_code lexer::handle_comments()
        {
            std::size_t starting_line = this->M_line;
            this->pre_advance_lexer();
            while (!this->has_reached_eof() && this->M_ch != '`')
            {
                this->pre_advance_lexer();
            }
            if (this->M_ch != '`')
            {
                this->M_current_lexer = this->M_start_lexer;
                this->M_line = starting_line;
                return horizon_errors::error_code::HORIZON_MISSING_TERMINATING_CHAR;
            }
            this->pre_advance_lexer(); // skip `
            return horizon_errors::error_code::HORIZON_NO_ERROR;
        }

        horizon_errors::error_code lexer::handle_char()
        {
            horizon_deps::string temp_str = "";
            this->pre_advance_lexer();
            bool was_invalid_es = false;
            while (!this->has_reached_eof())
            {
                if (this->M_ch == '\'' || this->M_ch == '\n')
                    break;
                else if (this->M_ch == '\\')
                {
                    this->pre_advance_lexer();
                    switch (this->M_ch)
                    {
                    case 'a':
                        temp_str += "\a";
                        break;
                    case 'b':
                        temp_str += "\b";
                        break;
                    case 'f':
                        temp_str += "\f";
                        break;
                    case 'n':
                        temp_str += "\n";
                        break;
                    case 'r':
                        temp_str += "\r";
                        break;
                    case 't':
                        temp_str += "\t";
                        break;
                    case 'v':
                        temp_str += "\v";
                        break;
                    case '\\':
                        temp_str += "\\";
                        break;
                    case '\'':
                        temp_str += "'";
                        break;
                    case '"':
                        temp_str += "\"";
                        break;
                    case '?':
                        temp_str += "\?";
                        break;
                    case '0':
                        temp_str.resize(temp_str.length() + 2);
                        temp_str.raw()[temp_str.length()] = 0;
                        temp_str.length() = temp_str.length() + 1;
                        break;

                    default:
                        temp_str += this->M_ch;
                        was_invalid_es = true;
                        this->M_invalid_ec = this->M_ch;
                        break;
                    }
                }
                else
                    temp_str += this->M_ch;
                this->pre_advance_lexer();
            }
            if (this->M_ch != '\'')
            {
                return horizon_errors::error_code::HORIZON_MISSING_TERMINATING_CHAR;
            }
            else if (temp_str.length() == 0)
            {
                this->pre_advance_lexer();
                return horizon_errors::error_code::HORIZON_LESS_BYTES;
            }
            else if (was_invalid_es)
            {
                this->pre_advance_lexer();
                return horizon_errors::error_code::HORIZON_INVALID_ESCAPE_SEQUENCE;
            }
            else if (temp_str.length() == 1)
            {
                this->pre_advance_lexer();
                this->M_tokens.add(token{token_type::TOKEN_CHAR_LITERAL, std::move(temp_str), this->M_start_lexer, this->M_current_lexer});
                return horizon_errors::error_code::HORIZON_NO_ERROR;
            }
            else
            {
                this->pre_advance_lexer();
                this->M_tokens.add(token{token_type::TOKEN_INTEGER_LITERAL, std::move(horizon_deps::string::to_string(temp_str.multichar_uint())), this->M_start_lexer, this->M_current_lexer});
                return horizon_errors::error_code::HORIZON_MORE_BYTES;
            }
        }

        horizon_errors::error_code lexer::handle_string()
        {
            horizon_deps::string temp_str = "";
            bool was_invalid_es = false;
            this->pre_advance_lexer();
            while (!this->has_reached_eof())
            {
                if (this->M_ch == '"' || this->M_ch == '\n')
                    break;
                else if (this->M_ch == '\\')
                {
                    this->pre_advance_lexer();
                    switch (this->M_ch)
                    {
                    case 'a':
                        temp_str += "\a";
                        break;
                    case 'b':
                        temp_str += "\b";
                        break;
                    case 'f':
                        temp_str += "\f";
                        break;
                    case 'n':
                        temp_str += "\n";
                        break;
                    case 'r':
                        temp_str += "\r";
                        break;
                    case 't':
                        temp_str += "\t";
                        break;
                    case 'v':
                        temp_str += "\v";
                        break;
                    case '\\':
                        temp_str += "\\";
                        break;
                    case '\'':
                        temp_str += "'";
                        break;
                    case '"':
                        temp_str += "\"";
                        break;
                    case '?':
                        temp_str += "\?";
                        break;
                    case '0':
                        temp_str.resize(temp_str.length() + 2);
                        temp_str.raw()[temp_str.length()] = 0;
                        temp_str.length() = temp_str.length() + 1;
                        break;

                    default:
                        temp_str += this->M_ch;
                        was_invalid_es = true;
                        this->M_invalid_ec = this->M_ch;
                        break;
                    }
                }
                else
                    temp_str += this->M_ch;
                this->pre_advance_lexer();
            }
            if (this->M_ch != '"')
                return horizon_errors::error_code::HORIZON_MISSING_TERMINATING_CHAR;
            this->pre_advance_lexer();
            this->M_tokens.add(token{token_type::TOKEN_STRING_LITERAL, std::move(temp_str), this->M_start_lexer, this->M_current_lexer});
            if (was_invalid_es)
                return horizon_errors::error_code::HORIZON_INVALID_ESCAPE_SEQUENCE;
            return horizon_errors::error_code::HORIZON_NO_ERROR;
        }

        std::size_t lexer::check_brackets() const
        {
            std::stack<std::pair<std::size_t, token_type>> s;

            for (std::size_t i = 0; i < this->M_tokens.length(); ++i)
            {
                const token_type &type = this->M_tokens[i].M_type;
                if (type == token_type::TOKEN_LEFT_PAREN || type == token_type::TOKEN_LEFT_BRACKET || type == token_type::TOKEN_LEFT_BRACE)
                {
                    s.push({i, type});
                }
                else if (type == token_type::TOKEN_RIGHT_PAREN || type == token_type::TOKEN_RIGHT_BRACKET || type == token_type::TOKEN_RIGHT_BRACE)
                {
                    if (s.empty())
                        return i;

                    std::pair<std::size_t, token_type> top = s.top(); // copy is created
                    s.pop();

                    if ((type == token_type::TOKEN_RIGHT_PAREN && top.second != token_type::TOKEN_LEFT_PAREN) ||
                        (type == token_type::TOKEN_RIGHT_BRACKET && top.second != token_type::TOKEN_LEFT_BRACKET) ||
                        (type == token_type::TOKEN_RIGHT_BRACE && top.second != token_type::TOKEN_LEFT_BRACE))
                    {
                        return i;
                    }
                }
            }

            return (s.empty()) ? static_cast<std::size_t>(-1) : s.top().first;
        }

        lexer::lexer(horizon_misc::HR_FILE *file)
        {
            this->M_file = file;
            this->M_line = 1;
            this->M_current_lexer = 0;
            this->M_start_lexer = 0;
        }

        bool lexer::init_lexing()
        {
            this->M_ch = this->M_file->M_content[this->M_current_lexer];
            if (!this->scan_tokens())
                return false;
            this->M_tokens.add(token{token_type::TOKEN_END_OF_FILE, nullptr, static_cast<std::size_t>(-1), static_cast<std::size_t>(-1)});
            std::size_t invalid_bracket_pos = this->check_brackets();
            if (invalid_bracket_pos != static_cast<std::size_t>(-1))
            {
                horizon_errors::errors::lexer_draw_error(horizon_errors::error_code::HORIZON_INVALID_BRACKET, this->M_file, horizon_errors::errors::getline_no(this->M_file->M_content, this->M_tokens[invalid_bracket_pos].M_start),
                                                         this->M_tokens[invalid_bracket_pos].M_start,
                                                         this->M_tokens[invalid_bracket_pos].M_end, {"invalid or unexpected bracket", this->M_tokens[invalid_bracket_pos].M_lexeme});
                return false;
            }
            this->M_tokens.shrink_to_fit();
            return true;
        }

        const horizon_deps::vector<token> &lexer::get() const
        {
            return this->M_tokens;
        }

        horizon_deps::vector<token> &lexer::get()
        {
            return this->M_tokens;
        }

        horizon_deps::vector<token> &&lexer::move()
        {
            return std::move(this->M_tokens);
        }

        void lexer::debug_print() const
        {
            const char *to_str[] =
                {
                    "TOKEN_IDENTIFIER",
                    "TOKEN_CHAR_LITERAL",
                    "TOKEN_STRING_LITERAL",
                    "TOKEN_INTEGER_LITERAL",
                    "TOKEN_DECIMAL_LITERAL",
                    "TOKEN_ARITHMETIC_ADD",
                    "TOKEN_ARITHMETIC_SUBSTRACT",
                    "TOKEN_ARITHMETIC_MULTIPLY",
                    "TOKEN_ARITHMETIC_POWER",
                    "TOKEN_ARITHMETIC_DIVIDE",
                    "TOKEN_ARITHMETIC_MODULUS",
                    "TOKEN_RELATIONAL_EQUAL_TO",
                    "TOKEN_RELATIONAL_NOT_EQUAL_TO",
                    "TOKEN_RELATIONAL_GREATER_THAN",
                    "TOKEN_RELATIONAL_LESS_THAN",
                    "TOKEN_RELATIONAL_GREATER_THAN_OR_EQUAL_TO",
                    "TOKEN_RELATIONAL_LESS_THAN_OR_EQUAL_TO",
                    "TOKEN_LOGICAL_NOT",
                    "TOKEN_LOGICAL_AND",
                    "TOKEN_LOGICAL_OR",
                    "TOKEN_BITWISE_NOT",
                    "TOKEN_BITWISE_AND",
                    "TOKEN_BITWISE_OR",
                    "TOKEN_BITWISE_XOR",
                    "TOKEN_BITWISE_LEFT_SHIFT",
                    "TOKEN_BITWISE_RIGHT_SHIFT",
                    "TOKEN_ASSIGN",
                    "TOKEN_ASSIGN_ADD",
                    "TOKEN_ASSIGN_SUBSTRACT",
                    "TOKEN_ASSIGN_MULTIPLY",
                    "TOKEN_ASSIGN_DIVIDE",
                    "TOKEN_ASSIGN_MODULUS",
                    "TOKEN_ASSIGN_BITWISE_AND",
                    "TOKEN_ASSIGN_BITWISE_OR",
                    "TOKEN_ASSIGN_BITWISE_XOR",
                    "TOKEN_ASSIGN_LEFT_SHIFT",
                    "TOKEN_ASSIGN_RIGHT_SHIFT",
                    "TOKEN_INCREMENT",
                    "TOKEN_DECREMENT",
                    "TOKEN_QUESTION",
                    "TOKEN_SEMICOLON",
                    "TOKEN_COLON",
                    "TOKEN_COMMA",
                    "TOKEN_DOT",
                    "TOKEN_RIGHT_PAREN",
                    "TOKEN_LEFT_PAREN",
                    "TOKEN_RIGHT_BRACE",
                    "TOKEN_LEFT_BRACE",
                    "TOKEN_RIGHT_BRACKET",
                    "TOKEN_LEFT_BRACKET",
                    "TOKEN_KEYWORD",
                    "TOKEN_PRIMARY_TYPE",
                    "TOKEN_END_OF_FILE"};

            for (std::size_t i = 0; i < this->M_tokens.length(); i++)
            {
                std::printf("'%s': %s: start:%zu, end:%zu\n", (this->M_tokens[i].M_lexeme == "\n" ? "\\n" : this->M_tokens[i].M_lexeme.c_str()), to_str[static_cast<std::size_t>(this->M_tokens[i].M_type)], this->M_tokens[i].M_start, this->M_tokens[i].M_end);
            }
        }
    }
}