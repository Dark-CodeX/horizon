/**
 * @file sptr.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_DEPS_SPTR_SPTR_HH
#define HORIZON_DEPS_SPTR_SPTR_HH

#include "../../src/misc/exit_heap_fail.hh"

namespace horizon
{
    namespace horizon_deps
    {
        template <typename T>
        class sptr
        {
        private:
            T *M_ptr;

        public:
            sptr();
            sptr(const T &s);
            sptr(T &&s) noexcept(true);
            sptr(const sptr &p);
            sptr(sptr &&p) noexcept(true);
            sptr(T *p);
            [[nodiscard]] bool is_null() const;
            bool release();
            [[nodiscard]] T &get();
            [[nodiscard]] const T &get() const;
            [[nodiscard]] const T *raw() const;
            [[nodiscard]] T *&raw();
            sptr &operator=(const T &s);
            sptr &operator=(T &&s) noexcept(true);
            sptr &operator=(const sptr &p);
            sptr &operator=(sptr &&p) noexcept(true);
            sptr &operator=(T *p);
            operator bool() const;
            const T *operator->() const;
            T *&operator->();
            const T &operator*() const;
            T &operator*();
            ~sptr();
        };

        template <typename T>
        sptr<T>::sptr()
        {
            this->M_ptr = nullptr;
        }

        template <typename T>
        sptr<T>::sptr(const T &s)
        {
            this->M_ptr = new T(s);
            horizon_misc::exit_heap_fail(this->M_ptr, "horizon::horizon_deps::sptr");
        }

        template <typename T>
        sptr<T>::sptr(T &&s) noexcept(true)
        {
            this->M_ptr = new T(std::move(s));
            horizon_misc::exit_heap_fail(this->M_ptr, "horizon::horizon_deps::sptr");
        }

        template <typename T>
        sptr<T>::sptr(const sptr &p)
        {
            this->M_ptr = new T(*p.M_ptr);
            horizon_misc::exit_heap_fail(this->M_ptr, "horizon::horizon_deps::sptr");
        }

        template <typename T>
        sptr<T>::sptr(sptr &&p) noexcept(true)
        {
            this->M_ptr = p.M_ptr;
            p.M_ptr = nullptr;
        }

        template <typename T>
        sptr<T>::sptr(T *p)
        {
            this->M_ptr = p;
            p = nullptr;
        }

        template <typename T>
        bool sptr<T>::is_null() const
        {
            return this->M_ptr == nullptr;
        }

        template <typename T>
        bool sptr<T>::release()
        {
            if (this->M_ptr)
            {
                delete this->M_ptr;
                this->M_ptr = nullptr;
                return true;
            }
            return false;
        }

        template <typename T>
        T &sptr<T>::get()
        {
            return *this->M_ptr;
        }

        template <typename T>
        const T &sptr<T>::get() const
        {
            return *this->M_ptr;
        }

        template <typename T>
        const T *sptr<T>::raw() const
        {
            return this->M_ptr;
        }

        template <typename T>
        T *&sptr<T>::raw()
        {
            return this->M_ptr;
        }

        template <typename T>
        sptr<T> &sptr<T>::operator=(const T &s)
        {
            this->release();
            this->M_ptr = new T(s);
            horizon_misc::exit_heap_fail(this->M_ptr, "horizon::horizon_deps::sptr");
            return *this;
        }

        template <typename T>
        sptr<T> &sptr<T>::operator=(T &&s) noexcept(true)
        {
            this->release();
            this->M_ptr = new T(std::move(s));
            horizon_misc::exit_heap_fail(this->M_ptr, "horizon::horizon_deps::sptr");
            return *this;
        }

        template <typename T>
        sptr<T> &sptr<T>::operator=(const sptr &p)
        {
            if (this != &p)
            {
                this->release();
                this->M_ptr = new T(*p.M_ptr);
                horizon_misc::exit_heap_fail(this->M_ptr, "horizon::horizon_deps::sptr");
            }
            return *this;
        }

        template <typename T>
        sptr<T> &sptr<T>::operator=(sptr &&p) noexcept(true)
        {
            if (this != &p)
            {
                this->release();
                this->M_ptr = p.M_ptr;
                p.M_ptr = nullptr;
            }
            return *this;
        }

        template <typename T>
        sptr<T> &sptr<T>::operator=(T *p)
        {
            this->release();
            this->M_ptr = p;
            return *this;
        }

        template <typename T>
        sptr<T>::operator bool() const
        {
            return !this->is_null(); // is_null is true if M_ptr is null so we have to inverse it
        }

        template <typename T>
        const T *sptr<T>::operator->() const
        {
            return this->M_ptr;
        }

        template <typename T>
        T *&sptr<T>::operator->()
        {
            return this->M_ptr;
        }

        template <typename T>
        const T &sptr<T>::operator*() const
        {
            return *this->M_ptr;
        }

        template <typename T>
        T &sptr<T>::operator*()
        {
            return *this->M_ptr;
        }

        template <typename T>
        sptr<T>::~sptr()
        {
            if (this->M_ptr)
            {
                delete this->M_ptr;
                this->M_ptr = nullptr;
            }
        }
    }
}

#endif