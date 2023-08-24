/**
 * @file horizon.cc
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#include "../lexer/lexer.hh"
#include "../parser/parser.hh"
#include "../misc/load_file.hh"

int main(int argc, char **argv)
{
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    make_prompt_colored();
#endif

    if (argc == 1)
    {
        if (COLOR_ERR)
            std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " no file given\n");
        else
            std::fprintf(stderr, "horizon: error: no file given\n");
        return EXIT_FAILURE;
    }

    horizon::horizon_deps::sptr<horizon::horizon_misc::HR_FILE> file = horizon::horizon_misc::load_file(argv[1]);
    if (!file)
    {
        // error message is already printed and memory is freed
        return EXIT_FAILURE;
    }

    horizon::horizon_deps::sptr<horizon::horizon_lexer::lexer> lexer(file.raw());
    if (!lexer->init_lexing())
    {
        return EXIT_FAILURE;
    }

    horizon::horizon_deps::sptr<horizon::horizon_parser::parser> parser({std::move(lexer->move()), file.raw()});

    if (!parser->init_parsing())
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}