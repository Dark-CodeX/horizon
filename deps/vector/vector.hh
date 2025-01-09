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
            T *data;
            std::size_t len, cap;

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
            [[nodiscard]] const T &operator[](const std::size_t &nth) const;
            [[nodiscard]] T &operator[](const std::size_t &nth);
            vector &operator=(const vector &vec);
            vector &operator=(vector &&__s) noexcept(true);
            ~vector();
        };

        template <typename T>
        void vector<T>::init_vector(const std::size_t &N)
        {
            this->data = new T[N];
            horizon_misc::exit_heap_fail(this->data, "horizon::horizon_deps::vector");
            this->cap = N;
            this->len = 0;
        }

        template <typename T>
        void vector<T>::resize_vector()
        {
            this->cap *= 2;
            T *temp = new T[this->cap];
            horizon_misc::exit_heap_fail(temp, "horizon::horizon_deps::vector");
            for (std::size_t i = 0; i < this->len; i++)
                temp[i] = std::move(this->data[i]);
            delete[] this->data;
            this->data = temp;
        }

        template <typename T>
        vector<T>::vector()
        {
            this->data = nullptr;
            this->len = 0;
            this->cap = 0;
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
                this->len = 0;
                this->cap = 0;
                this->data = nullptr;
            }
        }

        template <typename T>
        vector<T>::vector(T *ptr, const std::size_t &__len)
        {
            if (ptr)
            {
                this->data = ptr;
                this->cap = this->len = __len;
            }
            else
            {
                this->len = 0;
                this->cap = 0;
                this->data = nullptr;
            }
        }

        template <typename T>
        vector<T>::vector(const vector<T> &vec)
        {
            if (vec.data)
            {
                this->init_vector(vec.cap);
                for (std::size_t i = 0; i < vec.len; i++)
                    this->add(vec.data[i]);
            }
            else
            {
                this->len = 0;
                this->cap = 0;
                this->data = nullptr;
            }
        }

        template <typename T>
        vector<T>::vector(vector &&other) noexcept(true)
        {
            this->data = other.data;
            this->len = other.len;
            this->cap = other.cap;

            other.data = nullptr;
            other.len = 0;
            other.cap = 0;
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
            return this->len;
        }

        template <typename T>
        const std::size_t &vector<T>::capacity() const
        {
            return this->cap;
        }

        template <typename T>
        vector<T> &vector<T>::add(const T &item)
        {
            if (!this->data)
                this->init_vector(16);
            if (this->len == this->cap)
                this->resize_vector();
            this->data[this->len++] = item;
            return *this;
        }

        template <typename T>
        vector<T> &vector<T>::add(T &&item)
        {
            if (!this->data)
                this->init_vector(16);
            if (this->len == this->cap)
                this->resize_vector();
            this->data[this->len++] = std::move(item);
            return *this;
        }

        template <typename T>
        vector<T> &vector<T>::remove()
        {
            if (!this->data || this->len == 0)
                return *this;
            this->data[--this->len].~T();
            return *this;
        }

        template <typename T>
        vector<T> &vector<T>::remove(const std::size_t &nth)
        {
            if (!this->data || this->len == 0)
                return *this;
            if (nth >= this->len)
            {
                if (COLOR_ERR)
                    std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " " ENCLOSE(WHITE_FG, "horizon::horizon_deps::vector:") " invalid memory access in %p for %zu, max was %zu\n", static_cast<void *>(this->data), nth, this->len);
                else
                    std::fprintf(stderr, "horizon: error: horizon::horizon_deps::vector: invalid memory access in %p for %zu, max was %zu\n", static_cast<void *>(this->data), nth, this->len);
                std::exit(EXIT_FAILURE);
            }
            for (std::size_t i = nth; i < this->len - 1; i++)
            {
                this->data[i] = std::move(this->data[i + 1]);
            }
            this->len--;
            return *this;
        }

        template <typename T>
        bool vector<T>::is_empty() const
        {
            return this->len == 0;
        }

        template <typename T>
        bool vector<T>::is_null() const
        {
            return this->data == nullptr;
        }

        template <typename T>
        vector<T> &vector<T>::erase()
        {
            if (this->data)
                delete[] this->data;
            this->data = nullptr;
            this->len = 0;
            this->cap = 0;
            return *this;
        }

        template <typename T>
        vector<T> &vector<T>::shrink_to_fit()
        {
            if (!this->data)
                return *this;
            T *temp = new T[this->len];
            horizon_misc::exit_heap_fail(temp, "horizon::horizon_deps::vector");
            for (std::size_t i = 0; i < this->len; i++)
                temp[i] = std::move(this->data[i]);
            this->cap = this->len;
            delete[] this->data;
            this->data = temp;
            return *this;
        }

        template <typename T>
        const T *vector<T>::raw() const
        {
            return this->data;
        }

        template <typename T>
        T *&vector<T>::raw()
        {
            return this->data;
        }

        template <typename T>
        const T &vector<T>::operator[](const std::size_t &nth) const
        {
            if (nth < this->len && this->data)
                return this->data[nth];
            if (COLOR_ERR)
                std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " " ENCLOSE(WHITE_FG, "horizon::horizon_deps::vector:") " invalid memory access in %p for %zu, max was %zu\n", static_cast<void *>(this->data), nth, this->len);
            else
                std::fprintf(stderr, "horizon: error: horizon::horizon_deps::vector: invalid memory access in %p for %zu, max was %zu\n", static_cast<void *>(this->data), nth, this->len);
            std::exit(EXIT_FAILURE);
        }

        template <typename T>
        T &vector<T>::operator[](const std::size_t &nth)
        {
            if (nth < this->len && this->data)
                return this->data[nth];
            if (COLOR_ERR)
                std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " " ENCLOSE(WHITE_FG, "horizon::horizon_deps::vector:") " invalid memory access in %p for %zu, max was %zu\n", static_cast<void *>(this->data), nth, this->len);
            else
                std::fprintf(stderr, "horizon: error: horizon::horizon_deps::vector: invalid memory access in %p for %zu, max was %zu\n", static_cast<void *>(this->data), nth, this->len);
            std::exit(EXIT_FAILURE);
        }

        template <typename T>
        vector<T> &vector<T>::operator=(const vector &vec)
        {
            if (this != &vec)
            {
                if (this->data)
                    delete[] this->data;
                this->cap = vec.cap;
                this->init_vector(this->cap);
                this->len = vec.len;
                for (std::size_t i = 0; i < vec.len; i++)
                    this->data[i] = vec.data[i];
            }
            return *this;
        }

        template <typename T>
        vector<T> &vector<T>::operator=(vector &&__s) noexcept(true)
        {
            if (this != &__s)
            {
                if (this->data)
                    delete[] this->data;
                this->data = __s.data;
                this->cap = __s.cap;
                this->len = __s.len;

                __s.data = nullptr;
                __s.len = 0;
                __s.cap = 0;
            }
            return *this;
        }

        template <typename T>
        vector<T>::~vector()
        {
            if (this->data)
                delete[] this->data;
            this->data = nullptr;
            this->len = 0;
            this->cap = 0;
        }
    }
}

#endif