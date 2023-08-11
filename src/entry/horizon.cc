/**
 * @file horizon.cc
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#include "../lexer/lexer.hh"
#include "../parser/parser.hh"
#include "../colorize/colorize.h"

int main(int argc, char **argv)
{
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    make_prompt_colored();
#endif

    if (argc == 1)
    {
        if (COLOR_ERR)
            std::fprintf(stderr, ENCLOSE(RED_FG, "error:") " no file given\n");
        else
            std::fprintf(stderr, "error: no file given\n");
        return EXIT_FAILURE;
    }
    horizon::horizon_lexer::lexer lexer(argv[1]);
    if (!lexer.init_lexing())
    {
        return EXIT_FAILURE;
    }
    lexer.debug_print();
    return EXIT_SUCCESS;
}