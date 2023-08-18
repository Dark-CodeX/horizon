/**
 * @file horizon.cc
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#include "../lexer/lexer.hh"
#include "../parser/parser.hh"
#include "../misc/misc.hh"

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

    horizon::horizon_misc::HR_FILE *file = horizon::horizon_misc::misc::load_file(argv[1]);
    if (!file)
    {
        // error message is already printed and memory is freed
        return EXIT_FAILURE;
    }

    horizon::horizon_lexer::lexer *lexer = new horizon::horizon_lexer::lexer(file);
    horizon::horizon_misc::misc::exit_heap_fail(lexer, "horizon::horizon_lexer");
    if (!lexer->init_lexing())
    {
        delete lexer;
        delete file;
        return EXIT_FAILURE;
    }

    horizon::horizon_parser::parser *parser = new horizon::horizon_parser::parser(std::move(lexer->move()), file);
    horizon::horizon_misc::misc::exit_heap_fail(parser, "horizon::horizon_parser");
    delete lexer;

    if (!parser->init_parsing())
    {
        delete parser;
        delete file;
        return EXIT_FAILURE;
    }

    delete parser;
    delete file;

    return EXIT_SUCCESS;
}