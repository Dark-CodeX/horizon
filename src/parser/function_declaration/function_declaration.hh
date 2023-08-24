/**
 * @file function_declaration.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_PARSER_FUNCTION_DECLARATION_HH
#define HORIZON_PARSER_FUNCTION_DECLARATION_HH

#include <map>

#include "../../../deps/string/string.hh"

namespace horizon
{
    namespace horizon_parser
    {
        struct function_parameter_list
        {
            //       1. Identifier         2. Type
            std::map<horizon_deps::string, horizon_deps::vector<horizon_deps::string>> M_container;
        };

        struct function_template_list
        {
        };

        struct function_declaration
        {
            horizon_deps::string M_identifier;
            function_parameter_list M_parameter;
            bool M_is_template;
            function_template_list M_template;
            horizon_deps::vector<horizon_deps::string> M_return_type;
        };
    }
}

#endif