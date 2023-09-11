/**
 * @file misc.cc
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#include "./load_file.hh"
#include "./exit_heap_fail.hh"
#include "./is_directory.hh"

namespace horizon
{
    namespace horizon_misc
    {
        horizon_deps::sptr<HR_FILE> load_file(const char *loc)
        {
            if (!loc)
            {
                if (COLOR_ERR)
                    std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " horizon::horizon_misc::load_file: file location was (null)\n");
                else
                    std::fprintf(stderr, "horizon: error: horizon::horizon_misc::load_file: file location was (null)\n");
                return nullptr;
            }
            if (is_directory(loc))
            {
                if (COLOR_ERR)
                    std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error[E1]:") " '%s' cannot be opened for reading: Is a directory\n", loc);
                else
                    std::fprintf(stderr, "horizon: error[E1]: '%s' cannot be opened for reading: Is a directory\n", loc);
                return nullptr;
            }
            HR_FILE *file = new HR_FILE();
            exit_heap_fail(file, "horizon::horizon_misc::load_file");
            file->M_location = loc;
            std::FILE *fptr = std::fopen(loc, "rb");
            if (!fptr)
            {
                delete file;
                if (COLOR_ERR)
                    std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error[E1]:") " " ENCLOSE(WHITE_FG, "'%s'") " cannot be opened for reading: %s\n", loc, std::strerror(errno));
                else
                    std::fprintf(stderr, "horizon: error[E1]: '%s' cannot be opened for reading: %s\n", loc, std::strerror(errno));
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
                    std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error[E1]:") " " ENCLOSE(WHITE_FG, "'%s'") " not every byte was read: %s\n", loc, std::strerror(errno));
                else
                    std::fprintf(stderr, "horizon: error[E1]: '%s' not every byte was read: %s\n", loc, std::strerror(errno));
                return nullptr;
            }
            file->M_content.length() = LEN;
            std::fclose(fptr);
            return file;
        }

        void exit_heap_fail(const void *ptr, const char *__s)
        {
            if (!ptr)
            {
                if (COLOR_ERR)
                    std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " " ENCLOSE(WHITE_FG, "%s:") " cannot allocate memory on heap.\n", __s);
                else
                    std::fprintf(stderr, "horizon: error: %s: cannot allocate memory on heap.\n", __s);
                std::exit(EXIT_FAILURE);
            }
        }

        bool is_directory(const char *loc)
        {
            if (!loc)
                return false;
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
            DWORD fileAttributes = GetFileAttributesA(loc);
            if (fileAttributes == INVALID_FILE_ATTRIBUTES)
                return false;
            else if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                return true;
            else
                return false;
#else
            struct stat buffer;
            if (stat(loc, &buffer) == 0 && S_ISDIR(buffer.st_mode))
                return true;
#endif
            return false;
        }
    }
}
