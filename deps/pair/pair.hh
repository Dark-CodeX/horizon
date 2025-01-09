/**
 * @file pair.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_DEPS_PAIR_PAIR_HH
#define HORIZON_DEPS_PAIR_PAIR_HH

#include "../../src/misc/exit_heap_fail.hh"

namespace horizon
{
    namespace horizon_deps
    {
        template <typename T, typename U = T>
        class pair
        {
          private:
            T *ptr1;
            U *ptr2;

          public:
            pair();
            pair(const T &p1, const U &p2);
            pair(T &&p1, U &&p2) noexcept(true);
            pair(const pair &p);
            pair(pair &&p) noexcept(true);
            pair(T *p1, U *p2);
            [[nodiscard]] bool is_null() const;
            bool release();
            [[nodiscard]] T &get_first();
            [[nodiscard]] const T &get_first() const;
            [[nodiscard]] U &get_second();
            [[nodiscard]] const U &get_second() const;
            [[nodiscard]] const T *raw_first() const;
            [[nodiscard]] T *&raw_first();
            [[nodiscard]] const U *raw_second() const;
            [[nodiscard]] U *&raw_second();
            pair &operator=(const pair &p);
            pair &operator=(pair &&p) noexcept(true);
            operator bool() const;
            ~pair();
        };

        template <typename T, typename U>
        pair<T, U>::pair()
        {
            this->ptr1 = nullptr;
            this->ptr2 = nullptr;
        }

        template <typename T, typename U>
        pair<T, U>::pair(const T &p1, const U &p2)
        {
            this->ptr1 = new T(p1);
            this->ptr2 = new U(p2);
            horizon_misc::exit_heap_fail(this->ptr1, "horizon::horizon_deps::pair");
            horizon_misc::exit_heap_fail(this->ptr2, "horizon::horizon_deps::pair");
        }

        template <typename T, typename U>
        pair<T, U>::pair(T &&p1, U &&p2) noexcept(true)
        {
            this->ptr1 = new T(std::move(p1));
            this->ptr2 = new U(std::move(p2));
            horizon_misc::exit_heap_fail(this->ptr1, "horizon::horizon_deps::pair");
            horizon_misc::exit_heap_fail(this->ptr2, "horizon::horizon_deps::pair");
        }

        template <typename T, typename U>
        pair<T, U>::pair(const pair &p)
        {
            this->ptr1 = new T(*p.ptr1);
            this->ptr2 = new U(*p.ptr2);
            horizon_misc::exit_heap_fail(this->ptr1, "horizon::horizon_deps::pair");
            horizon_misc::exit_heap_fail(this->ptr2, "horizon::horizon_deps::pair");
        }

        template <typename T, typename U>
        pair<T, U>::pair(pair &&p) noexcept(true)
        {
            this->ptr1 = p.ptr1;
            this->ptr2 = p.ptr2;

            p.ptr1 = nullptr;
            p.ptr2 = nullptr;
        }

        template <typename T, typename U>
        pair<T, U>::pair(T *p1, U *p2)
        {
            this->ptr1 = p1;
            this->ptr2 = p2;

            p1 = nullptr;
            p2 = nullptr;
        }

        template <typename T, typename U>
        bool pair<T, U>::is_null() const
        {
            return this->ptr1 == nullptr && this->ptr2 == nullptr;
        }

        template <typename T, typename U>
        bool pair<T, U>::release()
        {
            bool is_done = false;
            if (this->ptr1)
            {
                delete this->ptr1;
                this->ptr1 = nullptr;
                is_done = true;
            }
            if (this->ptr2)
            {
                delete this->ptr2;
                this->ptr2 = nullptr;
                is_done = true;
            }
            return is_done;
        }

        template <typename T, typename U>
        T &pair<T, U>::get_first()
        {
            return *this->ptr1;
        }

        template <typename T, typename U>
        const T &pair<T, U>::get_first() const
        {
            return *this->ptr1;
        }

        template <typename T, typename U>
        U &pair<T, U>::get_second()
        {
            return *this->ptr2;
        }

        template <typename T, typename U>
        const U &pair<T, U>::get_second() const
        {
            return *this->ptr2;
        }

        template <typename T, typename U>
        const T *pair<T, U>::raw_first() const
        {
            return this->ptr1;
        }

        template <typename T, typename U>
        T *&pair<T, U>::raw_first()
        {
            return this->ptr1;
        }

        template <typename T, typename U>
        const U *pair<T, U>::raw_second() const
        {
            return this->ptr2;
        }

        template <typename T, typename U>
        U *&pair<T, U>::raw_second()
        {
            return this->ptr2;
        }

        template <typename T, typename U>
        pair<T, U> &pair<T, U>::operator=(const pair &p)
        {
            if (this != &p)
            {
                this->release();
                this->ptr1 = new T(*p.ptr1);
                this->ptr2 = new U(*p.ptr2);
                horizon_misc::exit_heap_fail(this->ptr1, "horizon::horizon_deps::pair");
                horizon_misc::exit_heap_fail(this->ptr2, "horizon::horizon_deps::pair");
            }
            return *this;
        }

        template <typename T, typename U>
        pair<T, U> &pair<T, U>::operator=(pair &&p) noexcept(true)
        {

            if (this != &p)
            {
                this->release();
                this->ptr1 = p.ptr1;
                this->ptr2 = p.ptr2;

                p.ptr1 = nullptr;
                p.ptr2 = nullptr;
            }
            return *this;
        }

        template <typename T, typename U>
        pair<T, U>::operator bool() const
        {
            return !this->is_null();
        }

        template <typename T, typename U>
        pair<T, U>::~pair()
        {
            if (this->ptr1)
            {
                delete this->ptr1;
                this->ptr1 = nullptr;
            }
            if (this->ptr2)
            {
                delete this->ptr2;
                this->ptr2 = nullptr;
            }
        }
    }
}
#endif