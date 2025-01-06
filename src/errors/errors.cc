/**
 * @file errors.cc
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#include "./errors.hh"

namespace horizon
{
    namespace horizon_errors
    {
        std::pair<horizon_deps::string, std::size_t> errors::getline(const horizon_deps::string &str, const std::size_t &start, const std::size_t &end__, const horizon_deps::string &color)
        {
            std::size_t end = (start == end__ ? end__ + 1 : end__);
            horizon_deps::string line_middle;
            if (COLOR_ERR)
                line_middle = color + str.substr(start, end - start) + RESET_COLOR;
            else
                line_middle = str.substr(start, end - start);

            std::size_t line_start_pos = start;
            for (; str[line_start_pos] != '\n' && line_start_pos > 0; line_start_pos--)
                ;
            if (str[line_start_pos] == '\n')
                line_start_pos++;
            horizon_deps::string line_start = str.substr(line_start_pos, start - line_start_pos);

            std::size_t line_end_pos = end;
            for (; str[line_end_pos] != '\n' && line_end_pos < str.length(); line_end_pos++)
                ;
            horizon_deps::string line_end = str.substr(end, line_end_pos - end);

            return {line_start + line_middle + line_end, start - line_start_pos};
        }

        std::size_t errors::getline_no(const horizon_deps::string &str, const std::size_t &start)
        {
            std::size_t cline = 1;
            for (std::size_t i = 0; i < start; i++)
            {
                if (str[i] == '\n')
                    cline++;
            }
            return cline;
        }

        void errors::lexer_draw_error(const error_code &code, const horizon_misc::HR_FILE *file, const std::size_t &line_no, const std::size_t &start, const std::size_t &end, const horizon_deps::vector<horizon_deps::string> &err_msg)
        {
            std::pair<horizon_deps::string, std::size_t> data = errors::getline(file->M_content, start, end, RED_FG);
            if (COLOR_ERR)
                std::fprintf(stderr, "horizon: lexer: " ENCLOSE(WHITE_FG, "%s:%zu:%zu:") " " ENCLOSE(RED_FG, "error[E%u]:") " ", file->M_location.c_str(), line_no, data.second + 1, (unsigned)code);
            else
                std::fprintf(stderr, "horizon: lexer: %s:%zu:%zu: error[E%u]: ", file->M_location.c_str(), line_no, data.second + 1, (unsigned)code);

            for (std::size_t i = 0; i < err_msg.length(); i++)
                std::fprintf(stderr, "%s%s", err_msg[i].c_str(), (i < err_msg.length() - 1 ? " " : "\n"));

            int x = std::fprintf(stderr, "  %zu", line_no);
            std::fprintf(stderr, " | %s\n", data.first.c_str());
            if (COLOR_ERR)
                std::fprintf(stderr, "%s | %s" RED_FG "^%s" RESET_COLOR "\n", horizon_deps::string(' ', x).c_str(), horizon_deps::string(' ', data.second).c_str(), (start == end ? "" : horizon_deps::string('~', end - start - 1).c_str()));
            else
                std::fprintf(stderr, "%s | %s^%s\n", horizon_deps::string(' ', x).c_str(), horizon_deps::string(' ', data.second).c_str(), (start == end ? "" : horizon_deps::string('~', end - start - 1).c_str()));
        }

        void errors::parser_draw_error(const error_code &code, const horizon_misc::HR_FILE *file, const token &tok, const horizon_deps::vector<horizon_deps::string> &err_msg)
        {
            std::pair<horizon_deps::string, std::size_t> data = errors::getline(file->M_content, tok.M_start, tok.M_end, RED_FG);
            std::size_t line_no = errors::getline_no(file->M_content, tok.M_start);

            if (COLOR_ERR)
                std::fprintf(stderr, "horizon: parser: " ENCLOSE(WHITE_FG, "%s:%zu:%zu:") " " ENCLOSE(RED_FG, "error[E%u]:") " ", file->M_location.c_str(), line_no, data.second + 1, (unsigned)code);
            else
                std::fprintf(stderr, "horizon: parser: %s:%zu:%zu: error[E%u]: ", file->M_location.c_str(), line_no, data.second + 1, (unsigned)code);

            for (std::size_t i = 0; i < err_msg.length(); i++)
                std::fprintf(stderr, "%s%s", err_msg[i].c_str(), (i < err_msg.length() - 1 ? " " : "\n"));

            int x = std::fprintf(stderr, "  %zu", line_no);
            std::fprintf(stderr, " | %s\n", data.first.c_str());
            if (COLOR_ERR)
                std::fprintf(stderr, "%s | %s" RED_FG "^%s" RESET_COLOR "\n", horizon_deps::string(' ', x).c_str(), horizon_deps::string(' ', data.second).c_str(), (tok.M_start == tok.M_end ? "" : horizon_deps::string('~', tok.M_end - tok.M_start - 1).c_str()));
            else
                std::fprintf(stderr, "%s | %s^%s\n", horizon_deps::string(' ', x).c_str(), horizon_deps::string(' ', data.second).c_str(), (tok.M_start == tok.M_end ? "" : horizon_deps::string('~', tok.M_end - tok.M_start - 1).c_str()));
        }
    }
}