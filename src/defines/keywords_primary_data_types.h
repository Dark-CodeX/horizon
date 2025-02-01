/**
 * @file keywords_primary_data_types.h
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_DEFINES_KEYWORDS_PRIMARY_DATA_TYPES_H
#define HORIZON_DEFINES_KEYWORDS_PRIMARY_DATA_TYPES_H

#include <cstring>

static const char *horizon_keywords[] = {
    "let",
    "func",
    "sizeof",
    "operator",
    "heap",
    "array",
    "export",
    "public",
    "private",
    "import",
    "const",
    "ref",
    "this",
    "block",
    "enum",
    "return",
    "if",
    "else",
    "elif",
    "null",
    "throw",
    "struct",
    "typeof",
    "alias",
    "true",
    "false",
    "static",
    "for",
    "while",
    "do",
    "match",
    "case",
    "break",
    "continue"};

static const char *horizon_primary_data_types[] = {
    "char",
    "uchar",
    "u8char",
    "u16char",
    "u32char",
    "str",
    "ustr",
    "u8str",
    "u16str",
    "u32str",
    "bool",
    "int8",
    "uint8",
    "int16",
    "uint16",
    "int32",
    "uint32",
    "int64",
    "uint64",
    "dec32",
    "dec64",
    "dec128",
    "void"};

int is_keyword(const char *str);

int is_primary_data_type(const char *str);

#endif