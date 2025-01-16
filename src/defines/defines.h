/**
 * @file defines.h
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_DEFINES_DEFINES_H
#define HORIZON_DEFINES_DEFINES_H

#define RED_FG "\033[1;91m"
#define GREEN_FG "\033[1;92m"
#define YELLOW_FG "\033[1;93m"
#define BLUE_FG "\033[1;94m"
#define PURPLE_FG "\033[1;95m"
#define CYAN_FG "\033[1;96m"
#define WHITE_FG "\033[1;97m"
#define RESET_COLOR "\033[0m"

#define ENCLOSE(clr, txt) \
    clr txt RESET_COLOR

#endif