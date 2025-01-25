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
                    std::cout << PURPLE_FG << (this->M_val.c_str() == nullptr ? "(null)" : this->M_val.c_str()) << RESET_COLOR;
                else
                    std::cout << GREEN_FG << this->M_val << RESET_COLOR;
            }
        };

        class ast_unary_operation_node : public ast_node
        {
            horizon_deps::sptr<ast_node> M_operand;
            token M_operator;
            bool M_is_prefix;

          public:
            inline ast_unary_operation_node(horizon_deps::sptr<ast_node> &&operand, token &&opr, bool prefix)
                : M_operand(std::move(operand)), M_operator(std::move(opr)), M_is_prefix(prefix) {}

            inline void print() const override
            {
                printf("( ");
                if (this->M_is_prefix)
                {
                    std::cout << BLUE_FG << this->M_operator.M_lexeme.c_str() << RESET_COLOR " ";
                    if (this->M_operand)
                        this->M_operand->print();
                }
                else
                {
                    if (this->M_operand)
                        this->M_operand->print();
                    std::cout << " " << BLUE_FG << this->M_operator.M_lexeme.c_str() << RESET_COLOR;
                }
                printf(" )");
            }
        };

        class ast_binary_operation_node : public ast_node
        {
            horizon_deps::sptr<ast_node> M_left;
            token M_operator;
            horizon_deps::sptr<ast_node> M_right;

          public:
            inline ast_binary_operation_node(horizon_deps::sptr<ast_node> &&left, token &&opr, horizon_deps::sptr<ast_node> &&right)
                : M_left(std::move(left)), M_operator(std::move(opr)), M_right(std::move(right)) {}

            inline void print() const override
            {
                printf("( ");
                this->M_left->print();
                std::cout << " " << BLUE_FG << this->M_operator.M_lexeme.c_str() << RESET_COLOR " ";
                this->M_right->print();
                printf(" )");
            }
        };

        class ast_data_type_node : public ast_node
        {
            horizon_deps::vector<token> M_type_qualifiers;
            horizon_deps::sptr<ast_node> M_type;

          public:
            inline ast_data_type_node(horizon_deps::vector<token> &&type_qual, horizon_deps::sptr<ast_node> &&type_)
                : M_type_qualifiers(std::move(type_qual)), M_type(std::move(type_)) {}

            inline void print() const override
            {
                if (!this->M_type_qualifiers.is_empty())
                {
                    for (const token &i : this->M_type_qualifiers)
                    {
                        std::cout << RED_FG << i.M_lexeme.c_str() << RESET_COLOR " ";
                    }
                }
                if (this->M_type)
                    this->M_type->print();
            }
        };

        class ast_ternary_operator_node : public ast_node
        {
            horizon_deps::sptr<ast_node> M_condition;
            horizon_deps::sptr<ast_node> M_val_if_true;
            horizon_deps::sptr<ast_node> M_val_if_false;

          public:
            inline ast_ternary_operator_node(horizon_deps::sptr<ast_node> &&cond, horizon_deps::sptr<ast_node> &&if_true, horizon_deps::sptr<ast_node> &&if_false)
                : M_condition(std::move(cond)), M_val_if_true(std::move(if_true)), M_val_if_false(std::move(if_false)) {}

            inline void print() const override
            {
                std::cout << "TERNARY: CONDITION: ";
                if (this->M_condition)
                    this->M_condition->print();
                std::cout << " VALUE_IF_TRUE: ";
                if (this->M_val_if_true)
                    this->M_val_if_true->print();
                std::cout << " VALUE_IF_FALSE: ";
                if (this->M_val_if_false)
                    this->M_val_if_false->print();
                std::cout << "\n";
            }
        };

        class ast_variable_declaration_node : public ast_node
        {
            horizon_deps::sptr<ast_node> M_type;
            horizon_deps::vector<horizon_deps::pair<token, horizon_deps::sptr<ast_node>>> M_variables;

          public:
            inline ast_variable_declaration_node(horizon_deps::sptr<ast_node> &&type, horizon_deps::vector<horizon_deps::pair<token, horizon_deps::sptr<ast_node>>> &&vars)
                : M_type(std::move(type)), M_variables(std::move(vars)) {}

            inline void print() const override
            {
                std::cout << "VAR_DECL TYPE: ";
                if (this->M_type)
                    this->M_type->print();
                std::cout << "(\n";
                for (const horizon_deps::pair<token, horizon_deps::sptr<ast_node>> &i : this->M_variables)
                {
                    std::cout << "\tNAME: " << PURPLE_FG << (i.get_first().M_lexeme.c_str() == nullptr ? "(null)" : i.get_first().M_lexeme.c_str()) << RESET_COLOR "    VALUE: ";
                    if (i.raw_second())
                        i.get_second()->print();
                    std::cout << "\n";
                }
                std::cout << ")\n";
            }
        };

        class ast_function_call_node : public ast_node
        {
            token M_identifier;
            horizon_deps::vector<horizon_deps::sptr<ast_node>> M_arguments;

          public:
            inline ast_function_call_node(token &&identifier, horizon_deps::vector<horizon_deps::sptr<ast_node>> &&args)
                : M_identifier(std::move(identifier)), M_arguments(std::move(args)) {}

            inline void print() const override
            {
                std::cout << "CALL NAME: " << PURPLE_FG << this->M_identifier.M_lexeme.c_str() << RESET_COLOR "( ";
                for (std::size_t i = 0; i < this->M_arguments.length(); i++)
                {
                    if (this->M_arguments[i])
                    {
                        this->M_arguments[i]->print();
                        std::cout << (i < this->M_arguments.length() - 1 ? ", " : " ");
                    }
                }
                std::cout << ")";
            }
        };

        class ast_block_node : public ast_node
        {
            horizon_deps::vector<horizon_deps::sptr<ast_node>> M_nodes;

          public:
            inline ast_block_node(horizon_deps::vector<horizon_deps::sptr<ast_node>> &&nodes)
                : M_nodes(std::move(nodes)) {}

            inline void print() const override
            {
                std::cout << "BLOCK {\n";
                for (std::size_t i = 0; i < this->M_nodes.length(); i++)
                {

                    if (this->M_nodes[i])
                    {
                        std::cout << YELLOW_FG << i << RESET_COLOR "\n";
                        this->M_nodes[i]->print();
                        std::cout << "\n";
                    }
                }
                std::cout << "}\n";
            }
        };

        class ast_if_elif_else_node : public ast_node
        {
            horizon_deps::pair<horizon_deps::sptr<ast_node>> M_if_condition_block;
            horizon_deps::vector<horizon_deps::pair<horizon_deps::sptr<ast_node>>> M_elif_condition_block;
            horizon_deps::sptr<ast_node> M_else_block;

          public:
            inline ast_if_elif_else_node(horizon_deps::pair<horizon_deps::sptr<ast_node>> &&if_cond_block, horizon_deps::vector<horizon_deps::pair<horizon_deps::sptr<ast_node>>> &&elif_cond_block, horizon_deps::sptr<ast_node> &&else_block)
                : M_if_condition_block(std::move(if_cond_block)), M_elif_condition_block(std::move(elif_cond_block)), M_else_block(std::move(else_block)) {}

            inline void print() const override
            {
                std::cout << ENCLOSE(RED_FG, "IF ");
                if (this->M_if_condition_block)
                {
                    this->M_if_condition_block.get_first()->print();
                    std::cout << " ";
                    this->M_if_condition_block.get_second()->print();
                }

                for (std::size_t i = 0; i < this->M_elif_condition_block.length(); i++)
                {
                    if (this->M_elif_condition_block[i])
                    {
                        std::cout << ENCLOSE(RED_FG, "ELIF ");
                        this->M_elif_condition_block[i].get_first()->print();
                        std::cout << " ";
                        this->M_elif_condition_block[i].get_second()->print();
                    }
                }

                if (this->M_else_block)
                {
                    std::cout << ENCLOSE(RED_FG, "ELSE ");
                    this->M_else_block->print();
                }
            }
        };

        class ast_for_loop_node : public ast_node
        {
            horizon_deps::sptr<ast_node> M_variable_decl;
            horizon_deps::sptr<ast_node> M_condition;
            horizon_deps::sptr<ast_node> M_step;
            horizon_deps::sptr<ast_node> M_block;

          public:
            inline ast_for_loop_node(horizon_deps::sptr<ast_node> &&var_decl, horizon_deps::sptr<ast_node> &&condition, horizon_deps::sptr<ast_node> &&step, horizon_deps::sptr<ast_node> &&block)
                : M_variable_decl(std::move(var_decl)), M_condition(std::move(condition)), M_step(std::move(step)), M_block(std::move(block)) {}

            inline void print() const override
            {
                std::cout << ENCLOSE(RED_FG, "FOR LOOP ") << "(\n";
                if (this->M_variable_decl)
                    this->M_variable_decl->print();
                if (this->M_condition)
                {
                    std::cout << ENCLOSE(RED_FG, "CONDITION") << "\n";
                    this->M_condition->print();
                }
                if (this->M_step)
                {
                    std::cout << ENCLOSE(RED_FG, "\nSTEP") << "\n";
                    this->M_step->print();
                }
                if (this->M_block)
                {
                    std::cout << "\n";
                    this->M_block->print();
                }
                std::cout << ")\n";
            }
        };

        class ast_while_loop_node : public ast_node
        {
            horizon_deps::sptr<ast_node> M_condition;
            horizon_deps::sptr<ast_node> M_block;

          public:
            inline ast_while_loop_node(horizon_deps::sptr<ast_node> &&condition, horizon_deps::sptr<ast_node> &&block)
                : M_condition(std::move(condition)), M_block(std::move(block)) {}

            inline void print() const override
            {
                std::cout << ENCLOSE(RED_FG, "WHILE LOOP ") << "(\n";
                if (this->M_condition)
                {
                    std::cout << ENCLOSE(RED_FG, "CONDITION") << "\n";
                    this->M_condition->print();
                }
                if (this->M_block)
                {
                    std::cout << "\n";
                    this->M_block->print();
                }
                std::cout << ")\n";
            }
        };

        class ast_do_while_loop_node : public ast_node
        {
            horizon_deps::sptr<ast_node> M_block;
            horizon_deps::sptr<ast_node> M_condition;

          public:
            inline ast_do_while_loop_node(horizon_deps::sptr<ast_node> &&block, horizon_deps::sptr<ast_node> &&condition)
                : M_block(std::move(block)), M_condition(std::move(condition)) {}

            inline void print() const override
            {
                std::cout << ENCLOSE(RED_FG, "DO WHILE LOOP ") << "(\n";
                std::cout << ENCLOSE(RED_FG, "DO");
                if (this->M_block)
                {
                    std::cout << "\n";
                    this->M_block->print();
                }
                if (this->M_condition)
                {
                    std::cout << ENCLOSE(RED_FG, "CONDITION") << "\n";
                    this->M_condition->print();
                }
                std::cout << ")\n";
            }
        };

        class ast_jump_statement_node : public ast_node
        {
            token M_keyword;
            horizon_deps::sptr<ast_node> M_expression;

          public:
            inline ast_jump_statement_node(token &&keyword__, horizon_deps::sptr<ast_node> &&expr)
                : M_keyword(std::move(keyword__)), M_expression(std::move(expr)) {}

            inline void print() const override
            {
                std::cout << RED_FG << this->M_keyword.M_lexeme.c_str() << RESET_COLOR " ";
                if (this->M_expression)
                    this->M_expression->print();
            }
        };

        class ast_parameter_node : public ast_node
        {
            horizon_deps::vector<horizon_deps::pair<horizon_deps::sptr<ast_node>, horizon_deps::vector<horizon_deps::pair<token, horizon_deps::sptr<ast_node>>>>> M_parameters;

          public:
            inline ast_parameter_node(horizon_deps::vector<horizon_deps::pair<horizon_deps::sptr<ast_node>, horizon_deps::vector<horizon_deps::pair<token, horizon_deps::sptr<ast_node>>>>> &&params)
                : M_parameters(std::move(params)) {}

            inline void print() const override
            {
                std::cout << "(\n";
                for (std::size_t i = 0; i < this->M_parameters.length(); i++)
                {
                    if (this->M_parameters[i])
                    {
                        std::cout << YELLOW_FG << i << RESET_COLOR << "\tTYPE: ";
                        if (this->M_parameters[i].get_first())
                        {
                            this->M_parameters[i].get_first()->print();
                            std::cout << " (";
                        }
                        for (std::size_t j = 0; j < this->M_parameters[i].get_second().length(); j++)
                        {
                            std::cout << "NAME: " << PURPLE_FG << this->M_parameters[i].get_second()[j].get_first().M_lexeme.c_str() << RESET_COLOR " VALUE: ";
                            if (this->M_parameters[i].get_second()[j].raw_second())
                            {
                                this->M_parameters[i].get_second()[j].get_second()->print();
                                std::cout << (j < this->M_parameters[i].get_second().length() - 1 ? ", " : "");
                            }
                            else
                                std::cout << "(null)" << (j < this->M_parameters[i].get_second().length() - 1 ? ", " : "");
                        }
                        std::cout << " )\n";
                    }
                }
                std::cout << ")\n";
            }
        };

        class ast_function_declaration_node : public ast_node
        {
            token M_identifier;
            horizon_deps::sptr<ast_node> M_parameters;
            horizon_deps::sptr<ast_node> M_return_type;
            horizon_deps::sptr<ast_node> M_block;

          public:
            inline ast_function_declaration_node(token &&identifier, horizon_deps::sptr<ast_node> &&var_decl, horizon_deps::sptr<ast_node> &&return_type, horizon_deps::sptr<ast_node> &&block)
                : M_identifier(std::move(identifier)), M_parameters(std::move(var_decl)), M_return_type(std::move(return_type)), M_block(std::move(block)) {}

            inline void print() const override
            {
                std::cout << "FUNC_DECL NAME: " << PURPLE_FG << this->M_identifier.M_lexeme.c_str() << RESET_COLOR "(\nPARAMETERS:\n";
                if (this->M_parameters)
                    this->M_parameters->print();
                std::cout << "RETURN TYPE: ";
                if (this->M_return_type)
                {
                    this->M_return_type->print();
                    std::cout << "\n";
                }
                if (this->M_block)
                    this->M_block->print();
                std::cout << ")\n";
            }
        };
    }
}

#endif