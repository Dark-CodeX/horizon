/**
 * @file keywords_primary_data_types.cc
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#include "./keywords_primary_data_types.h"

int is_keyword(const char *str)
{
    if (!str)
        return false;
    for (size_t i = 0; i < (sizeof(horizon_keywords) / sizeof(*horizon_keywords)); i++)
        if (strcmp(str, horizon_keywords[i]) == 0)
            return true;
    return false;
}

int is_primary_data_type(const char *str)
{
    if (!str)
        return false;
    for (size_t i = 0; i < (sizeof(horizon_primary_data_types) / sizeof(*horizon_primary_data_types)); i++)
        if (strcmp(str, horizon_primary_data_types[i]) == 0)
            return true;
    return false;
}
