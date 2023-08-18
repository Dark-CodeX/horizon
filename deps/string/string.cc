/**
 * @file string.cc
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#include "./string.hh"

namespace horizon
{
    namespace horizon_deps
    {
        char *&string::str_catcpy(char *&dest, const char *src, std::size_t &updating_length)
        {
            if (!dest || !src)
                return dest;
            while ((dest[updating_length] = *src++))
                updating_length++;
            return dest;
        }

        char *&string::str_catcpy(char *&dest, const char &c, std::size_t &updating_length)
        {
            if (!dest)
                return dest;
            dest[updating_length] = c;
            updating_length++;
            return dest;
        }

        std::size_t string::str_len(const char *s)
        {
            if (s)
            {
                std::size_t i = 0;
                for (; s[i]; i++)
                    ;
                return i;
            }
            return 0;
        }

        char *&string::str_init_zero(char *&ptr, const std::size_t &begin, const std::size_t &end)
        {
            if (!ptr)
                return ptr;
            for (std::size_t i = begin; i < end; i++)
                ptr[i] = 0;
            return ptr;
        }

        bool string::str_cmp(const char *str1, const char *str2)
        {
            if (!str1 && !str2)
                return true;
            if (!str1 || !str2)
                return false;
            while (*str1 != 0 && *str2 != 0)
            {
                if (*str1 != *str2)
                    return false;
                str1++;
                str2++;
            }
            return (*str1 == 0 && *str2 == 0);
        }

        bool string::str_cmp(const char *str, const std::size_t &len, const char &c)
        {
            if (!str)
                return false;
            if (len != 1)
                return false;
            return *str == c;
        }

        string::string()
        {
            this->len = 0;
            this->str = nullptr;
        }

        string::string(const char &c)
        {
            this->len = 0;
            this->str = static_cast<char *>(std::calloc(2, sizeof(char)));
            horizon_misc::misc::exit_heap_fail(this->str, "horizon::horizon_deps::string");

            this->str = string::str_catcpy(this->str, c, this->len);
        }

        string::string(const char *src)
        {
            if (src)
            {
                this->len = 0;
                this->str = static_cast<char *>(std::calloc(string::str_len(src) + 1, sizeof(char)));
                horizon_misc::misc::exit_heap_fail(this->str, "horizon::horizon_deps::string");

                this->str = string::str_catcpy(this->str, src, this->len);
            }
            else
            {
                this->len = 0;
                this->str = nullptr;
            }
        }

        string::string(const string &src)
        {
            if (src.str)
            {
                this->len = 0;
                this->str = static_cast<char *>(std::calloc(src.len + 1, sizeof(char)));
                horizon_misc::misc::exit_heap_fail(this->str, "horizon::horizon_deps::string");

                this->str = string::str_catcpy(this->str, src.str, this->len);
            }
            else
            {
                this->len = 0;
                this->str = nullptr;
            }
        }

        string::string(string &&src) noexcept(true)
        {
            this->str = src.str;
            this->len = src.len;

            src.len = 0;
            src.str = nullptr;
        }

        string::string(const char *begin, const char *end)
        {
            if (begin && end)
            {
                this->len = 0;
                this->str = static_cast<char *>(std::calloc((static_cast<std::size_t>(end - begin)) + 1, sizeof(char)));
                horizon_misc::misc::exit_heap_fail(this->str, "horizon::horizon_deps::string");

                for (const char *ptr = begin; ptr != end; ptr++)
                    this->str[this->len++] = *ptr;
            }
            else
            {
                this->len = 0;
                this->str = nullptr;
            }
        }

        string::string(const char &c, const std::size_t &n)
        {
            this->len = 0;
            this->str = static_cast<char *>(std::calloc(n + 1, sizeof(char)));
            horizon_misc::misc::exit_heap_fail(this->str, "horizon::horizon_deps::string");

            for (; this->len < n; this->len++)
                this->str[this->len] = c;
        }

        string &string::assign(const char &c)
        {
            if (this->str)
            {
                std::free(this->str);
                this->str = nullptr;
            }
            this->len = 0;
            this->str = static_cast<char *>(std::calloc(2, sizeof(char)));
            horizon_misc::misc::exit_heap_fail(this->str, "horizon::horizon_deps::string");

            this->str = string::str_catcpy(this->str, c, this->len);
            return *this;
        }

        string &string::assign(const char *src)
        {
            if (this->str)
            {
                std::free(this->str);
                this->str = nullptr;
            }
            if (src)
            {
                this->len = 0;
                this->str = static_cast<char *>(std::calloc(string::str_len(src) + 1, sizeof(char)));
                horizon_misc::misc::exit_heap_fail(this->str, "horizon::horizon_deps::string");

                this->str = string::str_catcpy(this->str, src, this->len);
            }
            else
            {
                this->len = 0;
                this->str = nullptr;
            }
            return *this;
        }

        string &string::assign(const string &src)
        {
            if (this->str)
            {
                std::free(this->str);
                this->str = nullptr;
            }
            if (src.str)
            {
                this->len = 0;
                this->str = static_cast<char *>(std::calloc(src.len + 1, sizeof(char)));
                horizon_misc::misc::exit_heap_fail(this->str, "horizon::horizon_deps::string");

                this->str = string::str_catcpy(this->str, src.str, this->len);
            }
            else
            {
                this->len = 0;
                this->str = nullptr;
            }
            return *this;
        }

        string &string::assign(string &&src) noexcept(true)
        {
            if (this->str)
            {
                std::free(this->str);
                this->str = nullptr;
            }

            this->str = src.str;
            this->len = src.len;

            src.len = 0;
            src.str = nullptr;
            return *this;
        }

        string &string::append(const char &c)
        {
            if (this->str)
            {
                this->str = static_cast<char *>(std::realloc(this->str, (this->len + 2) * sizeof(char)));
                horizon_misc::misc::exit_heap_fail(this->str, "horizon::horizon_deps::string");
                this->str = string::str_init_zero(this->str, this->len, this->len + 2);

                this->str[this->len++] = c;
                return *this;
            }
            else
                return this->assign(c);
        }

        string &string::append(const char *src)
        {
            if (this->str)
            {
                if (src)
                {
                    std::size_t src_len = string::str_len(src);
                    this->str = static_cast<char *>(std::realloc(this->str, (this->len + src_len + 1) * sizeof(char)));
                    horizon_misc::misc::exit_heap_fail(this->str, "horizon::horizon_deps::string");
                    this->str = string::str_init_zero(this->str, this->len, this->len + src_len);

                    this->str = string::str_catcpy(this->str, src, this->len);
                }
                return *this;
            }
            else
                return this->assign(src);
        }

        string &string::append(const string &src)
        {
            if (this->str)
            {
                if (src.str)
                {
                    this->str = static_cast<char *>(std::realloc(this->str, (this->len + src.len + 1) * sizeof(char)));
                    horizon_misc::misc::exit_heap_fail(this->str, "horizon::horizon_deps::string");
                    this->str = string::str_init_zero(this->str, this->len, this->len + src.len);

                    this->str = string::str_catcpy(this->str, src.str, this->len);
                }
                return *this;
            }
            else
                return this->assign(src);
        }

        char *&string::raw()
        {
            return this->str;
        }

        const char *string::c_str() const
        {
            return this->str;
        }

        const std::size_t &string::length() const
        {
            return this->len;
        }

        std::size_t &string::length()
        {
            return this->len;
        }

        bool string::is_empty() const
        {
            return this->len == 0;
        }

        bool string::is_null() const
        {
            return this->str == nullptr;
        }

        bool string::compare(const char &c) const
        {
            return string::str_cmp(this->str, this->len, c);
        }

        bool string::compare(const char *src) const
        {
            return string::str_cmp(this->str, src);
        }

        bool string::compare(const string &src) const
        {
            return string::str_cmp(this->str, src.str);
        }

        string &string::clear()
        {
            if (this->str)
            {
                std::free(this->str);
                this->str = nullptr;
            }
            this->len = 0;
            return *this;
        }

        string &string::resize(const std::size_t &new_length)
        {
            if (new_length > 0 && new_length > this->len)
            {
                if (this->str)
                {
                    this->str = static_cast<char *>(std::realloc(this->str, sizeof(char) * new_length));
                    horizon_misc::misc::exit_heap_fail(this->str, "horizon::horizon_deps::string");
                    this->str = string::str_init_zero(this->str, this->len, new_length);
                }
                else
                {
                    this->str = static_cast<char *>(std::calloc(new_length, sizeof(char)));
                    horizon_misc::misc::exit_heap_fail(this->str, "horizon::horizon_deps::string");
                }
            }
            return *this;
        }

        string string::substr(const std::size_t &index, std::size_t sub_len) const
        {
            if (index >= this->len || !this->str)
                return nullptr;
            if (sub_len == static_cast<std::size_t>(-1) || index + sub_len > this->len)
                sub_len = this->len - index;
            char *buff = static_cast<char *>(std::calloc(sub_len + 1, sizeof(char)));
            horizon_misc::misc::exit_heap_fail(buff, "horizon::horizon_deps::string");

            std::memmove(buff, this->str + index, sub_len * sizeof(char));

            string ret;
            ret.str = buff;
            ret.len = sub_len;
            return ret;
        }

        std::size_t string::hash() const
        {
            std::size_t p = 53;
            std::size_t m = 1e9 + 9;
            long long power_of_p = 1;
            long long hash_val = 0;

            for (std::size_t i = 0; i < this->len; i++)
            {
                hash_val = (hash_val + (this->str[i] - 97 + 1) * power_of_p) % m;
                power_of_p = (power_of_p * p) % m;
            }
            return (hash_val % m + m) % m;
        }

        unsigned string::multichar_uint() const
        {
            if (!this->str)
                return 0;
            unsigned int val = 0;
            for (std::size_t i = 0; this->str[i]; i++)
                val = (val << 8) | this->str[i];
            return val;
        }

        const char &string::operator[](const std::size_t &__index) const
        {
            if (this->len >= __index && this->str)
                return this->str[__index];
            else
            {
                if (COLOR_ERR)
                    std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " " ENCLOSE(WHITE_FG, "horizon::horizon_deps::string:") " invalid memory access in %p for %zu, max was %zu\n", this->str, __index, this->len);
                else
                    std::fprintf(stderr, "horizon: error: horizon::horizon_deps::string: invalid memory access in %p for %zu, max was %zu\n", this->str, __index, this->len);
                std::exit(EXIT_FAILURE);
            }
        }

        char &string::operator[](const std::size_t &__index)
        {
            if (this->len >= __index && this->str)
                return this->str[__index];
            else
            {
                if (COLOR_ERR)
                    std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " " ENCLOSE(WHITE_FG, "horizon::horizon_deps::string:") " invalid memory access in %p for %zu, max was %zu\n", this->str, __index, this->len);
                else
                    std::fprintf(stderr, "horizon: error: horizon::horizon_deps::string: invalid memory access in %p for %zu, max was %zu\n", this->str, __index, this->len);
                std::exit(EXIT_FAILURE);
            }
        }

        string string::operator+(const char &c) const
        {
            string ret = string(*this);
            ret.append(c);
            return ret;
        }

        string string::operator+(const char *src) const
        {
            string ret = string(*this);
            ret.append(src);
            return ret;
        }

        string string::operator+(const string &src) const
        {
            string ret = string(*this);
            ret.append(src);
            return ret;
        }

        string &string::operator+=(const char &c)
        {
            return this->append(c);
        }

        string &string::operator+=(const char *src)
        {
            return this->append(src);
        }

        string &string::operator+=(const string &src)
        {
            return this->append(src);
        }

        string &string::operator=(const char &c)
        {
            return this->assign(c);
        }

        string &string::operator=(const char *src)
        {
            return this->assign(src);
        }

        string &string::operator=(const string &src)
        {
            if (this != &src)
                return this->assign(src);
            return *this;
        }

        string &string::operator=(string &&src) noexcept(true)
        {
            if (this != &src)
            {
                if (this->str)
                {
                    std::free(this->str);
                    this->str = nullptr;
                }

                this->str = src.str;
                this->len = src.len;

                src.len = 0;
                src.str = nullptr;
            }
            return *this;
        }

        int string::lexicographical_comparison(const char *src) const
        {
            if (!src)
                return this->len;
            if (!this->str)
                return this->len - string::str_len(src);
            std::size_t slen = 0;
            for (std::size_t i = 0; this->str[i] && src[slen]; i++, slen++)
            {
                if (this->str[i] == src[i])
                    continue;
                return (int)this->str[i] - (int)src[i];
            }
            return this->len - slen;
        }

        int string::lexicographical_comparison(const string &src) const
        {
            if (!src.str)
                return this->len;
            if (!this->str)
                return this->len - src.len;
            std::size_t slen = 0;
            for (std::size_t i = 0; this->str[i] && src[slen]; i++, slen++)
            {
                if (this->str[i] == src[i])
                    continue;
                return (int)this->str[i] - (int)src[i];
            }
            return this->len - slen;
        }

        bool string::operator==(const char &c) const
        {
            return this->compare(c);
        }

        bool string::operator==(const char *src) const
        {
            return this->compare(src);
        }

        bool string::operator==(const string &src) const
        {
            return this->compare(src);
        }

        bool string::operator!=(const char &c) const
        {
            return !this->compare(c);
        }

        bool string::operator!=(const char *src) const
        {
            return !this->compare(src);
        }

        bool string::operator!=(const string &src) const
        {
            return !this->compare(src);
        }

        bool string::operator<(const char *src) const
        {
            return this->lexicographical_comparison(src) < 0;
        }

        bool string::operator<(const string &src) const
        {
            return this->lexicographical_comparison(src) < 0;
        }

        string::~string()
        {
            if (this->str)
            {
                std::free(this->str);
                this->str = nullptr;
            }
            this->len = 0;
        }

        string string::to_string(const unsigned int &num)
        {
            char *ptr = static_cast<char *>(std::calloc(std::numeric_limits<unsigned int>::digits10 + 1, sizeof(char)));
            horizon_misc::misc::exit_heap_fail(ptr, "horizon::horizon_deps::string");
            int sn_len = std::snprintf(ptr, std::numeric_limits<unsigned int>::digits10, "%u", num);

            string ret;
            ret.str = ptr;
            ret.len = sn_len;
            return ret;
        }
    }
}
