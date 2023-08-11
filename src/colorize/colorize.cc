/**
 * @file colorize.cc
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#include "./colorize.h"

int should_colorize_err(void)
{
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    HANDLE h;
    DWORD m;

    h = GetStdHandle(STD_ERROR_HANDLE);
    return (h != INVALID_HANDLE_VALUE) && (h != NULL) && GetConsoleMode(h, &m);
#else
    char const *t = getenv("TERM");
    return t && strcmp(t, "dumb") != 0 && isatty(STDERR_FILENO);
#endif
}

int should_colorize_out(void)
{
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    HANDLE h;
    DWORD m;

    h = GetStdHandle(STD_OUTPUT_HANDLE);
    return (h != INVALID_HANDLE_VALUE) && (h != NULL) && GetConsoleMode(h, &m);
#else
    char const *t = getenv("TERM");
    return t && strcmp(t, "dumb") != 0 && isatty(STDOUT_FILENO);
#endif
}

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
void make_prompt_colored(void)
{
    if (COLOR_OUT)
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }
    if (COLOR_ERR)
    {
        HANDLE hOut = GetStdHandle(STD_ERROR_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }
}
#endif