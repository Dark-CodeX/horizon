/**
 * @file string.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#include "../../src/misc/misc.hh"

#ifndef HORIZON_DEPS_STRING_STRING_HH
#define HORIZON_DEPS_STRING_STRING_HH

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <limits>
#include <functional>

namespace horizon
{
    namespace horizon_deps
    {
        /**
         * This string class is only created to manage strings on heap memory efficiently.
         * RULES:
         *      1. Make it simple and raw
         *      2. Make only those functions which are really needed in the program
         *      3. USE `calloc` and `realloc` only
         *      4. If any function is missing and is really needed, make it yourself
         *      5. Main priority of this class is ONLY performance and efficiency both instruction and memory wise
         *      6. NO function should create my memory error or leaks
         */
        class string
        {
        private:
            static char *&str_catcpy(char *&dest, const char *src, std::size_t &updating_length);
            static char *&str_catcpy(char *&dest, const char &c, std::size_t &updating_length);
            static std::size_t str_len(const char *s);
            static char *&str_init_zero(char *&ptr, const std::size_t &begin, const std::size_t &end);
            static bool str_cmp(const char *str1, const char *str2);
            static bool str_cmp(const char *str, const std::size_t &len, const char &c);

        private:
            char *str;
            std::size_t len;

        public:
            string();
            string(const char &c);
            string(const char *src);
            string(const string &src);
            string(string &&src) noexcept(true);
            string(const char *begin, const char *end);
            string(const char &c, const std::size_t &n);
            string &assign(const char &c);
            string &assign(const char *src);
            string &assign(const string &src);
            string &assign(string &&src) noexcept(true);
            string &append(const char &c);
            string &append(const char *src);
            string &append(const string &src);
            [[nodiscard]] char *&raw();
            [[nodiscard]] const char *c_str() const;
            [[nodiscard]] const std::size_t &length() const;
            [[nodiscard]] std::size_t &length();
            [[nodiscard]] bool is_empty() const;
            [[nodiscard]] bool is_null() const;
            [[nodiscard]] bool compare(const char &c) const;
            [[nodiscard]] bool compare(const char *src) const;
            [[nodiscard]] bool compare(const string &src) const;
            string &clear();
            string &resize(const std::size_t &new_length);
            [[nodiscard]] string substr(const std::size_t &index, std::size_t sub_len = static_cast<std::size_t>(-1)) const;
            [[nodiscard]] std::size_t hash() const;
            [[nodiscard]] unsigned multichar_uint() const;
            [[nodiscard]] const char &operator[](const std::size_t &__index) const;
            [[nodiscard]] char &operator[](const std::size_t &__index);
            string operator+(const char &c) const;
            string operator+(const char *src) const;
            string operator+(const string &src) const;
            string &operator+=(const char &c);
            string &operator+=(const char *src);
            string &operator+=(const string &src);
            string &operator=(const char &c);
            string &operator=(const char *src);
            string &operator=(const string &src);
            string &operator=(string &&src) noexcept(true);
            [[nodiscard]] int lexicographical_comparison(const char *src) const;
            [[nodiscard]] int lexicographical_comparison(const string &src) const;
            [[nodiscard]] bool operator==(const char &c) const;
            [[nodiscard]] bool operator==(const char *src) const;
            [[nodiscard]] bool operator==(const string &src) const;
            [[nodiscard]] bool operator!=(const char &c) const;
            [[nodiscard]] bool operator!=(const char *src) const;
            [[nodiscard]] bool operator!=(const string &src) const;
            [[nodiscard]] bool operator<(const char *src) const;
            [[nodiscard]] bool operator<(const string &src) const;
            ~string();

            [[nodiscard]] static string to_string(const unsigned int &num);
        };
    }
}

namespace std
{
    template <>
    struct hash<horizon::horizon_deps::string>
    {
        std::size_t operator()(const horizon::horizon_deps::string &str) const
        {
            return str.hash();
        }
    };
}

#endif
