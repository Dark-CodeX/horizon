/**
 * @file colorize.h
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_COLORIZE_COLORIZE_H
#define HORIZON_COLORIZE_COLORIZE_H

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <cstdlib>
#include <cstring>
#include <cstdio>

int should_colorize_err(void);
int should_colorize_out(void);

const int COLOR_ERR = should_colorize_err();
const int COLOR_OUT = should_colorize_out();

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
void make_prompt_colored(void);
#endif

#endif