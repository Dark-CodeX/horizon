/**
 * @file ast.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_PARSER_AST_AST_HH
#define HORIZON_PARSER_AST_AST_HH

#include <iostream>

#include "../../../deps/sptr/sptr.hh"
#include "../../../deps/string/string.hh"
#include "../../token_type/token_type.hh"
#include "../../../deps/vector/vector.hh"
#include "../../../deps/pair/pair.hh"

namespace horizon
{
    namespace horizon_parser
    {
        class ast_node
        {
          public:
            virtual ~ast_node() = default;
            virtual void print() const = 0;
        };

        template <typename T>
        class ast_operand_node : public ast_node
        {
            T M_val;

          public:
            inline ast_operand_node(const T &val)
                : M_val(val) {}
            inline void print() const override
            {
                if constexpr (std::is_same<T, horizon_deps::string>::value)
                    std::cout << (this->M_val.c_str() == nullptr ? "(null)" : this->M_val.c_str());
                else
                    std::cout << this->M_val;
            }
        };

        class ast_binary_operation_node : public ast_node
        {
            horizon_deps::sptr<ast_node> M_left;
            token_type M_operator;
            horizon_deps::sptr<ast_node> M_right;

          public:
            inline ast_binary_operation_node(horizon_deps::sptr<ast_node> &&left, token_type opr, horizon_deps::sptr<ast_node> &&right)
                : M_left(std::move(left)), M_operator(opr), M_right(std::move(right)) {}

            inline void print() const override
            {
                const char *to_str[] =
                    {
                        "TOKEN_IDENTIFIER",
                        "TOKEN_CHAR_LITERAL",
                        "TOKEN_STRING_LITERAL",
                        "TOKEN_INTEGER_LITERAL",
                        "TOKEN_DECIMAL_LITERAL",
                        "+",
                        "-",
                        "*",
                        "**",
                        "/",
                        "%",
                        "==",
                        "!=",
                        ">",
                        "<",
                        ">=",
                        "<=",
                        "!",
                        "&&",
                        "||",
                        "~",
                        "&",
                        "|",
                        "^",
                        "<<",
                        ">>",
                        "=",
                        "+=",
                        "-=",
                        "*=",
                        "**=",
                        "/=",
                        "%=",
                        "&=",
                        "|=",
                        "^=",
                        "<<=",
                        ">>=",
                        "++",
                        "--",
                        "?",
                        ";",
                        ":",
                        ",",
                        ".",
                        ")",
                        "(",
                        "}",
                        "{",
                        "]",
                        "[",
                        "TOKEN_KEYWORD",
                        "TOKEN_PRIMARY_TYPE",
                        "TOKEN_END_OF_FILE"};
                printf("( ");
                this->M_left->print();
                std::cout << " " << to_str[(unsigned)this->M_operator] << " ";
                this->M_right->print();
                printf(" )");
            }
        };

        class ast_variable_declaration : public ast_node
        {
            horizon_deps::string M_type;
            horizon_deps::vector<horizon_deps::pair<horizon_deps::string, horizon_deps::sptr<ast_node>>> M_variables;

          public:
            inline ast_variable_declaration(horizon_deps::string &&type, horizon_deps::vector<horizon_deps::pair<horizon_deps::string, horizon_deps::sptr<ast_node>>> &&vars)
                : M_type(std::move(type)), M_variables(std::move(vars)) {}

            inline void print() const override
            {
                std::cout << "VAR_DECL TYPE: " << M_type.c_str() << "(\n";
                for (const horizon_deps::pair<horizon_deps::string, horizon_deps::sptr<ast_node>> &i : this->M_variables)
                {
                    std::cout << "\tNAME: " << (i.get_first().c_str() == nullptr ? "(null)" : i.get_first().c_str()) << "    VALUE: ";
                    if (i.raw_second())
                        i.get_second()->print();
                    std::cout << "\n";
                }
                std::cout << ")\n";
            }
        };

        class ast_function_call : public ast_node
        {
            horizon_deps::string M_identifier;
            horizon_deps::vector<horizon_deps::sptr<ast_node>> M_arguments;

          public:
            inline ast_function_call(horizon_deps::string &&identifier, horizon_deps::vector<horizon_deps::sptr<ast_node>> &&args)
                : M_identifier(std::move(identifier)), M_arguments(std::move(args)) {}

            inline void print() const override
            {
                std::cout << "CALL NAME: " << this->M_identifier.c_str() << "( ";
                for (const horizon_deps::sptr<ast_node> &i : this->M_arguments)
                {
                    if (i)
                    {
                        i->print();
                        std::cout << ",";
                    }
                }
                std::cout << " )";
            }
        };
    }
}

#endif