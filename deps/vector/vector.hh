/**
 * @file vector.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_DEPS_VECTOR_VECTOR_HH
#define HORIZON_DEPS_VECTOR_VECTOR_HH

#include <initializer_list>
#include <utility>
#include <cstdio>
#include <cstdlib>

#include "../../src/misc/exit_heap_fail.hh"

namespace horizon
{
    namespace horizon_deps
    {
        /**
         * This vector class is only created to manage items on heap memory efficiently.
         * RULES:
         *      1. Make it simple and raw
         *      2. Make only those functions which are really needed in the program
         *      3. If any function is missing and is really needed, make it yourself
         *      4. Main priority of this class is ONLY performance and efficiency both instruction and memory wise
         *      5. NO function should create any memory error or leaks
         */
        template <typename T>
        class vector
        {
          private:
            T *M_data;
            std::size_t M_len, M_cap;

          private:
            void init_vector(const std::size_t &N);
            void resize_vector();

          public:
            vector();
            vector(T *ptr_begin, T *ptr_end);
            vector(T *ptr, const std::size_t &__len);
            vector(const vector &vec);
            vector(vector &&other) noexcept(true);
            vector(const std::initializer_list<T> &__list);
            vector(std::initializer_list<T> &&__list);
            vector(const std::size_t &max_capacity);
            [[nodiscard]] const std::size_t &length() const;
            [[nodiscard]] const std::size_t &capacity() const;
            vector &add(const T &item);
            vector &add(T &&item);
            vector &remove();
            vector &remove(const std::size_t &nth);
            [[nodiscard]] bool is_empty() const;
            [[nodiscard]] bool is_null() const;
            vector &erase();
            vector &shrink_to_fit();
            [[nodiscard]] const T *raw() const;
            [[nodiscard]] T *&raw();
            [[nodiscard]] const T *begin() const;
            [[nodiscard]] T *&begin();
            [[nodiscard]] const T *end() const;
            [[nodiscard]] T *&end();
            [[nodiscard]] const T &operator[](const std::size_t &nth) const;
            [[nodiscard]] T &operator[](const std::size_t &nth);
            vector &operator=(const vector &vec);
            vector &operator=(vector &&__s) noexcept(true);
            ~vector();
        };

        template <typename T>
        void vector<T>::init_vector(const std::size_t &N)
        {
            this->M_data = new T[N];
            horizon_misc::exit_heap_fail(this->M_data, "horizon::horizon_deps::vector");
            this->M_cap = N;
            this->M_len = 0;
        }

        template <typename T>
        void vector<T>::resize_vector()
        {
            this->M_cap *= 2;
            T *temp = new T[this->M_cap];
            horizon_misc::exit_heap_fail(temp, "horizon::horizon_deps::vector");
            for (std::size_t i = 0; i < this->M_len; i++)
                temp[i] = std::move(this->M_data[i]);
            delete[] this->M_data;
            this->M_data = temp;
        }

        template <typename T>
        vector<T>::vector()
        {
            this->M_data = nullptr;
            this->M_len = 0;
            this->M_cap = 0;
        }

        template <typename T>
        vector<T>::vector(T *ptr_begin, T *ptr_end)
        {
            if (ptr_begin && ptr_end)
            {
                this->init_vector(16);
                for (const T *i = ptr_begin; i != ptr_end; i++)
                    this->add(*i);
            }
            else
            {
                this->M_len = 0;
                this->M_cap = 0;
                this->M_data = nullptr;
            }
        }

        template <typename T>
        vector<T>::vector(T *ptr, const std::size_t &__len)
        {
            if (ptr)
            {
                this->M_data = ptr;
                this->M_cap = this->M_len = __len;
            }
            else
            {
                this->M_len = 0;
                this->M_cap = 0;
                this->M_data = nullptr;
            }
        }

        template <typename T>
        vector<T>::vector(const vector<T> &vec)
        {
            if (vec.M_data)
            {
                this->init_vector(vec.M_cap);
                for (std::size_t i = 0; i < vec.M_len; i++)
                    this->add(vec.M_data[i]);
            }
            else
            {
                this->M_len = 0;
                this->M_cap = 0;
                this->M_data = nullptr;
            }
        }

        template <typename T>
        vector<T>::vector(vector &&other) noexcept(true)
        {
            this->M_data = other.M_data;
            this->M_len = other.M_len;
            this->M_cap = other.M_cap;

            other.M_data = nullptr;
            other.M_len = 0;
            other.M_cap = 0;
        }

        template <typename T>
        vector<T>::vector(const std::initializer_list<T> &__list)
        {
            this->init_vector(__list.size());
            for (const auto &i : __list)
                this->add(i);
        }

        template <typename T>
        vector<T>::vector(std::initializer_list<T> &&__list)
        {
            this->init_vector(__list.size());
            for (auto &&i : __list)
                this->add(std::move(i));
        }

        template <typename T>
        vector<T>::vector(const std::size_t &max_capacity)
        {
            this->init_vector(max_capacity);
        }

        template <typename T>
        const std::size_t &vector<T>::length() const
        {
            return this->M_len;
        }

        template <typename T>
        const std::size_t &vector<T>::capacity() const
        {
            return this->M_cap;
        }

        template <typename T>
        vector<T> &vector<T>::add(const T &item)
        {
            if (!this->M_data)
                this->init_vector(16);
            if (this->M_len == this->M_cap)
                this->resize_vector();
            this->M_data[this->M_len++] = item;
            return *this;
        }

        template <typename T>
        vector<T> &vector<T>::add(T &&item)
        {
            if (!this->M_data)
                this->init_vector(16);
            if (this->M_len == this->M_cap)
                this->resize_vector();
            this->M_data[this->M_len++] = std::move(item);
            return *this;
        }

        template <typename T>
        vector<T> &vector<T>::remove()
        {
            if (!this->M_data || this->M_len == 0)
                return *this;
            this->M_data[--this->M_len].~T();
            return *this;
        }

        template <typename T>
        vector<T> &vector<T>::remove(const std::size_t &nth)
        {
            if (!this->M_data || this->M_len == 0)
                return *this;
            if (nth >= this->M_len)
            {
                if (COLOR_ERR)
                    std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " " ENCLOSE(WHITE_FG, "horizon::horizon_deps::vector:") " invalid memory access in %p for %zu, max was %zu\n", static_cast<void *>(this->M_data), nth, this->M_len);
                else
                    std::fprintf(stderr, "horizon: error: horizon::horizon_deps::vector: invalid memory access in %p for %zu, max was %zu\n", static_cast<void *>(this->M_data), nth, this->M_len);
                std::exit(EXIT_FAILURE);
            }
            for (std::size_t i = nth; i < this->M_len - 1; i++)
            {
                this->M_data[i] = std::move(this->M_data[i + 1]);
            }
            this->M_len--;
            return *this;
        }

        template <typename T>
        bool vector<T>::is_empty() const
        {
            return this->M_len == 0;
        }

        template <typename T>
        bool vector<T>::is_null() const
        {
            return this->M_data == nullptr;
        }

        template <typename T>
        vector<T> &vector<T>::erase()
        {
            if (this->M_data)
                delete[] this->M_data;
            this->M_data = nullptr;
            this->M_len = 0;
            this->M_cap = 0;
            return *this;
        }

        template <typename T>
        vector<T> &vector<T>::shrink_to_fit()
        {
            if (!this->M_data)
                return *this;
            T *temp = new T[this->M_len];
            horizon_misc::exit_heap_fail(temp, "horizon::horizon_deps::vector");
            for (std::size_t i = 0; i < this->M_len; i++)
                temp[i] = std::move(this->M_data[i]);
            this->M_cap = this->M_len;
            delete[] this->M_data;
            this->M_data = temp;
            return *this;
        }

        template <typename T>
        const T *vector<T>::raw() const
        {
            return this->M_data;
        }

        template <typename T>
        T *&vector<T>::raw()
        {
            return this->M_data;
        }

        template <typename T>
        const T *vector<T>::begin() const
        {
            return this->M_data;
        }

        template <typename T>
        T *&vector<T>::begin()
        {
            return this->M_data;
        }

        template <typename T>
        const T *vector<T>::end() const
        {
            return this->M_data + this->M_len;
        }

        template <typename T>
        T *&vector<T>::end()
        {
            return this->M_data + this->M_len;
        }

        template <typename T>
        const T &vector<T>::operator[](const std::size_t &nth) const
        {
            if (nth < this->M_len && this->M_data)
                return this->M_data[nth];
            if (COLOR_ERR)
                std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " " ENCLOSE(WHITE_FG, "horizon::horizon_deps::vector:") " invalid memory access in %p for %zu, max was %zu\n", static_cast<void *>(this->M_data), nth, this->M_len);
            else
                std::fprintf(stderr, "horizon: error: horizon::horizon_deps::vector: invalid memory access in %p for %zu, max was %zu\n", static_cast<void *>(this->M_data), nth, this->M_len);
            std::exit(EXIT_FAILURE);
        }

        template <typename T>
        T &vector<T>::operator[](const std::size_t &nth)
        {
            if (nth < this->M_len && this->M_data)
                return this->M_data[nth];
            if (COLOR_ERR)
                std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " " ENCLOSE(WHITE_FG, "horizon::horizon_deps::vector:") " invalid memory access in %p for %zu, max was %zu\n", static_cast<void *>(this->M_data), nth, this->M_len);
            else
                std::fprintf(stderr, "horizon: error: horizon::horizon_deps::vector: invalid memory access in %p for %zu, max was %zu\n", static_cast<void *>(this->M_data), nth, this->M_len);
            std::exit(EXIT_FAILURE);
        }

        template <typename T>
        vector<T> &vector<T>::operator=(const vector &vec)
        {
            if (this != &vec)
            {
                if (this->M_data)
                    delete[] this->M_data;
                this->M_cap = vec.M_cap;
                this->init_vector(this->M_cap);
                this->M_len = vec.M_len;
                for (std::size_t i = 0; i < vec.M_len; i++)
                    this->M_data[i] = vec.M_data[i];
            }
            return *this;
        }

        template <typename T>
        vector<T> &vector<T>::operator=(vector &&__s) noexcept(true)
        {
            if (this != &__s)
            {
                if (this->M_data)
                    delete[] this->M_data;
                this->M_data = __s.M_data;
                this->M_cap = __s.M_cap;
                this->M_len = __s.M_len;

                __s.M_data = nullptr;
                __s.M_len = 0;
                __s.M_cap = 0;
            }
            return *this;
        }

        template <typename T>
        vector<T>::~vector()
        {
            if (this->M_data)
                delete[] this->M_data;
            this->M_data = nullptr;
            this->M_len = 0;
            this->M_cap = 0;
        }
    }
}

#endif