/**
 * @file misc.cc
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#include "./misc.hh"

namespace horizon
{
    namespace horizon_misc
    {
        HR_FILE *misc::load_file(const char *loc)
        {
            if (!loc)
            {
                std::fprintf(stderr, "error: horizon::horizon_misc::load_file: file location was (null)\n");
                return nullptr;
            }
            HR_FILE *file = new HR_FILE();
            if (!file)
            {
                std::fprintf(stderr, "error: horizon::horizon_misc::load_file: cannot allocate memory on heap.\n");
                std::exit(EXIT_FAILURE);
            }
            file->M_location = loc;
            std::FILE *fptr = std::fopen(loc, "rb");
            if (!fptr)
            {
                delete file;
                if (COLOR_ERR)
                    std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error[E0]:") " " ENCLOSE(WHITE_FG, "'%s'") " cannot be opened for reading: %s\n", loc, std::strerror(errno));
                else
                    std::fprintf(stderr, "horizon: error[E0]: '%s' cannot be opened for reading: %s\n", loc, std::strerror(errno));
                return nullptr;
            }

            std::fseek(fptr, 0, SEEK_END);
            std::size_t LEN = std::ftell(fptr);
            std::fseek(fptr, 0, SEEK_SET);

            file->M_content.resize(LEN + 1);
            if (std::fread(file->M_content.raw(), sizeof(char), LEN, fptr) != LEN)
            {
                std::fclose(fptr);
                delete file;
                if (COLOR_ERR)
                    std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error[E0]:") " " ENCLOSE(WHITE_FG, "'%s'") " not every byte was read: %s\n", loc, std::strerror(errno));
                else
                    std::fprintf(stderr, "horizon: error[E0]: '%s' not every byte was read: %s\n", loc, std::strerror(errno));
                return nullptr;
            }
            file->M_content.length() = LEN;
            std::fclose(fptr);
            return file;
        }
    }
}
