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
            this->M_len = 0;
            this->M_str = nullptr;
        }

        string::string(const char &c)
        {
            this->M_len = 0;
            this->M_str = static_cast<char *>(std::calloc(2, sizeof(char)));
            horizon_misc::exit_heap_fail(this->M_str, "horizon::horizon_deps::string");

            this->M_str = string::str_catcpy(this->M_str, c, this->M_len);
        }

        string::string(const char *src)
        {
            if (src)
            {
                this->M_len = 0;
                this->M_str = static_cast<char *>(std::calloc(string::str_len(src) + 1, sizeof(char)));
                horizon_misc::exit_heap_fail(this->M_str, "horizon::horizon_deps::string");

                this->M_str = string::str_catcpy(this->M_str, src, this->M_len);
            }
            else
            {
                this->M_len = 0;
                this->M_str = nullptr;
            }
        }

        string::string(const string &src)
        {
            if (src.M_str)
            {
                this->M_len = 0;
                this->M_str = static_cast<char *>(std::calloc(src.M_len + 1, sizeof(char)));
                horizon_misc::exit_heap_fail(this->M_str, "horizon::horizon_deps::string");

                this->M_str = string::str_catcpy(this->M_str, src.M_str, this->M_len);
            }
            else
            {
                this->M_len = 0;
                this->M_str = nullptr;
            }
        }

        string::string(string &&src) noexcept(true)
        {
            this->M_str = src.M_str;
            this->M_len = src.M_len;

            src.M_len = 0;
            src.M_str = nullptr;
        }

        string::string(const char *begin, const char *end)
        {
            if (begin && end)
            {
                this->M_len = 0;
                this->M_str = static_cast<char *>(std::calloc((static_cast<std::size_t>(end - begin)) + 1, sizeof(char)));
                horizon_misc::exit_heap_fail(this->M_str, "horizon::horizon_deps::string");

                for (const char *ptr = begin; ptr != end; ptr++)
                    this->M_str[this->M_len++] = *ptr;
            }
            else
            {
                this->M_len = 0;
                this->M_str = nullptr;
            }
        }

        string::string(const char &c, const std::size_t &n)
        {
            this->M_len = 0;
            this->M_str = static_cast<char *>(std::calloc(n + 1, sizeof(char)));
            horizon_misc::exit_heap_fail(this->M_str, "horizon::horizon_deps::string");

            for (; this->M_len < n; this->M_len++)
                this->M_str[this->M_len] = c;
        }

        string &string::assign(const char &c)
        {
            if (this->M_str)
            {
                std::free(this->M_str);
                this->M_str = nullptr;
            }
            this->M_len = 0;
            this->M_str = static_cast<char *>(std::calloc(2, sizeof(char)));
            horizon_misc::exit_heap_fail(this->M_str, "horizon::horizon_deps::string");

            this->M_str = string::str_catcpy(this->M_str, c, this->M_len);
            return *this;
        }

        string &string::assign(const char *src)
        {
            if (this->M_str)
            {
                std::free(this->M_str);
                this->M_str = nullptr;
            }
            if (src)
            {
                this->M_len = 0;
                this->M_str = static_cast<char *>(std::calloc(string::str_len(src) + 1, sizeof(char)));
                horizon_misc::exit_heap_fail(this->M_str, "horizon::horizon_deps::string");

                this->M_str = string::str_catcpy(this->M_str, src, this->M_len);
            }
            else
            {
                this->M_len = 0;
                this->M_str = nullptr;
            }
            return *this;
        }

        string &string::assign(const string &src)
        {
            if (this->M_str)
            {
                std::free(this->M_str);
                this->M_str = nullptr;
            }
            if (src.M_str)
            {
                this->M_len = 0;
                this->M_str = static_cast<char *>(std::calloc(src.M_len + 1, sizeof(char)));
                horizon_misc::exit_heap_fail(this->M_str, "horizon::horizon_deps::string");

                this->M_str = string::str_catcpy(this->M_str, src.M_str, this->M_len);
            }
            else
            {
                this->M_len = 0;
                this->M_str = nullptr;
            }
            return *this;
        }

        string &string::assign(string &&src) noexcept(true)
        {
            if (this->M_str)
            {
                std::free(this->M_str);
                this->M_str = nullptr;
            }

            this->M_str = src.M_str;
            this->M_len = src.M_len;

            src.M_len = 0;
            src.M_str = nullptr;
            return *this;
        }

        string &string::append(const char &c)
        {
            if (this->M_str)
            {
                this->M_str = static_cast<char *>(std::realloc(this->M_str, (this->M_len + 2) * sizeof(char)));
                horizon_misc::exit_heap_fail(this->M_str, "horizon::horizon_deps::string");
                this->M_str = string::str_init_zero(this->M_str, this->M_len, this->M_len + 2);

                this->M_str[this->M_len++] = c;
                return *this;
            }
            else
                return this->assign(c);
        }

        string &string::append(const char *src)
        {
            if (this->M_str)
            {
                if (src)
                {
                    std::size_t src_len = string::str_len(src);
                    this->M_str = static_cast<char *>(std::realloc(this->M_str, (this->M_len + src_len + 1) * sizeof(char)));
                    horizon_misc::exit_heap_fail(this->M_str, "horizon::horizon_deps::string");
                    this->M_str = string::str_init_zero(this->M_str, this->M_len, this->M_len + src_len);

                    this->M_str = string::str_catcpy(this->M_str, src, this->M_len);
                }
                return *this;
            }
            else
                return this->assign(src);
        }

        string &string::append(const string &src)
        {
            if (this->M_str)
            {
                if (src.M_str)
                {
                    this->M_str = static_cast<char *>(std::realloc(this->M_str, (this->M_len + src.M_len + 1) * sizeof(char)));
                    horizon_misc::exit_heap_fail(this->M_str, "horizon::horizon_deps::string");
                    this->M_str = string::str_init_zero(this->M_str, this->M_len, this->M_len + src.M_len);

                    this->M_str = string::str_catcpy(this->M_str, src.M_str, this->M_len);
                }
                return *this;
            }
            else
                return this->assign(src);
        }

        char *&string::raw()
        {
            return this->M_str;
        }

        const char *string::c_str() const
        {
            return this->M_str;
        }

        const std::size_t &string::length() const
        {
            return this->M_len;
        }

        std::size_t &string::length()
        {
            return this->M_len;
        }

        bool string::is_empty() const
        {
            return this->M_len == 0;
        }

        bool string::is_null() const
        {
            return this->M_str == nullptr;
        }

        bool string::compare(const char &c) const
        {
            return string::str_cmp(this->M_str, this->M_len, c);
        }

        bool string::compare(const char *src) const
        {
            return string::str_cmp(this->M_str, src);
        }

        bool string::compare(const string &src) const
        {
            return string::str_cmp(this->M_str, src.M_str);
        }

        string string::wrap(const char *__s) const
        {
            if (__s)
            {
                std::size_t __s_len = string::str_len(__s), b_len = 0;
                char *buffer = static_cast<char *>(std::calloc(__s_len * 2 + this->M_len + 1, sizeof(char)));
                horizon_misc::exit_heap_fail(buffer, "horizon::horizon_deps::string");
                string::str_catcpy(buffer, __s, b_len);
                string::str_catcpy(buffer, this->M_str, b_len);
                string::str_catcpy(buffer, __s, b_len);

                string val;
                val.M_str = buffer;
                val.M_len = b_len;
                return val;
            }
            return *this;
        }

        string string::wrap(const string &__s) const
        {
            if (__s.M_str)
            {
                std::size_t b_len = 0;
                char *buffer = static_cast<char *>(std::calloc(__s.M_len * 2 + this->M_len + 1, sizeof(char)));
                horizon_misc::exit_heap_fail(buffer, "horizon::horizon_deps::string");
                string::str_catcpy(buffer, __s.M_str, b_len);
                string::str_catcpy(buffer, this->M_str, b_len);
                string::str_catcpy(buffer, __s.M_str, b_len);

                string val;
                val.M_str = buffer;
                val.M_len = b_len;
                return val;
            }
            return *this;
        }

        string &string::clear()
        {
            if (this->M_str)
            {
                std::free(this->M_str);
                this->M_str = nullptr;
            }
            this->M_len = 0;
            return *this;
        }

        string &string::resize(const std::size_t &new_length)
        {
            if (new_length > 0 && new_length > this->M_len)
            {
                if (this->M_str)
                {
                    this->M_str = static_cast<char *>(std::realloc(this->M_str, sizeof(char) * new_length));
                    horizon_misc::exit_heap_fail(this->M_str, "horizon::horizon_deps::string");
                    this->M_str = string::str_init_zero(this->M_str, this->M_len, new_length);
                }
                else
                {
                    this->M_str = static_cast<char *>(std::calloc(new_length, sizeof(char)));
                    horizon_misc::exit_heap_fail(this->M_str, "horizon::horizon_deps::string");
                }
            }
            return *this;
        }

        string string::substr(const std::size_t &index, std::size_t sub_len) const
        {
            if (index >= this->M_len || !this->M_str)
                return nullptr;
            if (sub_len == static_cast<std::size_t>(-1) || index + sub_len > this->M_len)
                sub_len = this->M_len - index;
            char *buff = static_cast<char *>(std::calloc(sub_len + 1, sizeof(char)));
            horizon_misc::exit_heap_fail(buff, "horizon::horizon_deps::string");

            std::memmove(buff, this->M_str + index, sub_len * sizeof(char));

            string ret;
            ret.M_str = buff;
            ret.M_len = sub_len;
            return ret;
        }

        std::size_t string::hash() const
        {
            std::size_t p = 53;
            std::size_t m = 1e9 + 9;
            long long power_of_p = 1;
            long long hash_val = 0;

            for (std::size_t i = 0; i < this->M_len; i++)
            {
                hash_val = (hash_val + (this->M_str[i] - 97 + 1) * power_of_p) % m;
                power_of_p = (power_of_p * p) % m;
            }
            return (hash_val % m + m) % m;
        }

        unsigned string::multichar_uint() const
        {
            if (!this->M_str)
                return 0;
            unsigned int val = 0;
            for (std::size_t i = 0; this->M_str[i]; i++)
                val = (val << 8) | this->M_str[i];
            return val;
        }

        const char &string::operator[](const std::size_t &__index) const
        {
            if (this->M_len >= __index && this->M_str)
                return this->M_str[__index];
            else
            {
                if (COLOR_ERR)
                    std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " " ENCLOSE(WHITE_FG, "horizon::horizon_deps::string:") " invalid memory access in %p for %zu, max was %zu\n", this->M_str, __index, this->M_len);
                else
                    std::fprintf(stderr, "horizon: error: horizon::horizon_deps::string: invalid memory access in %p for %zu, max was %zu\n", this->M_str, __index, this->M_len);
                std::exit(EXIT_FAILURE);
            }
        }

        char &string::operator[](const std::size_t &__index)
        {
            if (this->M_len >= __index && this->M_str)
                return this->M_str[__index];
            else
            {
                if (COLOR_ERR)
                    std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " " ENCLOSE(WHITE_FG, "horizon::horizon_deps::string:") " invalid memory access in %p for %zu, max was %zu\n", this->M_str, __index, this->M_len);
                else
                    std::fprintf(stderr, "horizon: error: horizon::horizon_deps::string: invalid memory access in %p for %zu, max was %zu\n", this->M_str, __index, this->M_len);
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
                if (this->M_str)
                {
                    std::free(this->M_str);
                    this->M_str = nullptr;
                }

                this->M_str = src.M_str;
                this->M_len = src.M_len;

                src.M_len = 0;
                src.M_str = nullptr;
            }
            return *this;
        }

        int string::lexicographical_comparison(const char *src) const
        {
            if (!src)
                return this->M_len;
            if (!this->M_str)
                return this->M_len - string::str_len(src);
            std::size_t slen = 0;
            for (std::size_t i = 0; this->M_str[i] && src[slen]; i++, slen++)
            {
                if (this->M_str[i] == src[i])
                    continue;
                return (int)this->M_str[i] - (int)src[i];
            }
            return this->M_len - slen;
        }

        int string::lexicographical_comparison(const string &src) const
        {
            if (!src.M_str)
                return this->M_len;
            if (!this->M_str)
                return this->M_len - src.M_len;
            std::size_t slen = 0;
            for (std::size_t i = 0; this->M_str[i] && src[slen]; i++, slen++)
            {
                if (this->M_str[i] == src[i])
                    continue;
                return (int)this->M_str[i] - (int)src[i];
            }
            return this->M_len - slen;
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
            if (this->M_str)
            {
                std::free(this->M_str);
                this->M_str = nullptr;
            }
            this->M_len = 0;
        }

        string string::to_string(const unsigned int &num)
        {
            char *ptr = static_cast<char *>(std::calloc(std::numeric_limits<unsigned int>::digits10 + 1, sizeof(char)));
            horizon_misc::exit_heap_fail(ptr, "horizon::horizon_deps::string");
            int sn_len = std::snprintf(ptr, std::numeric_limits<unsigned int>::digits10, "%u", num);

            string ret;
            ret.M_str = ptr;
            ret.M_len = sn_len;
            return ret;
        }
    }
}
