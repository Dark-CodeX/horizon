/**
 * @file hashtable.hh
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#ifndef HORIZON_DEPS_HASHTABLE_HASHTABLE_HH
#define HORIZON_DEPS_HASHTABLE_HASHTABLE_HH

#include <functional>

#include "../../src/misc/exit_heap_fail.hh"
#include "../pair/pair.hh"

namespace horizon
{
    namespace horizon_deps
    {
        template <typename KEY, typename VALUE>
        struct node
        {
          public:
            pair<KEY, VALUE> M_element;
            node<KEY, VALUE> *M_next;
            node() = default;

            node(const pair<KEY, VALUE> &__p)
                : M_element(__p), M_next(nullptr) {}

            node(pair<KEY, VALUE> &&__p)
                : M_element(std::move(__p)), M_next(nullptr) {}
        };

        template <typename KEY, typename VALUE, typename hashing_function = std::hash<KEY>>
        class hashtable_iter;

        template <typename KEY, typename VALUE, typename hashing_function = std::hash<KEY>>
        class hashtable
        {
          private:
            node<KEY, VALUE> **M_table;
            std::size_t M_len, M_cap;

            friend class hashtable_iter<KEY, VALUE, hashing_function>;

            void init_map(const std::size_t &__c);
            void rehash();
            static std::size_t get_hash(const KEY &__k, const std::size_t &__c);

          public:
            hashtable();
            hashtable(const std::size_t &init_cap);
            hashtable(const hashtable &ht);
            hashtable(hashtable &&ht) noexcept(true);

            [[nodiscard]] bool append(const KEY &__k, const VALUE &__v);
            [[nodiscard]] bool append(KEY &&__k, VALUE &&__v);
            [[nodiscard]] bool append(const pair<KEY, VALUE> &__p);
            [[nodiscard]] bool append(pair<KEY, VALUE> &&__p);

            [[nodiscard]] bool remove(const KEY &__k);
            [[nodiscard]] bool is_null() const;
            [[nodiscard]] bool is_empty() const;
            [[nodiscard]] bool compare(const hashtable &ht) const;
            hashtable &erase();

            typedef hashtable_iter<KEY, VALUE, hashing_function> iter;

            [[nodiscard]] const std::size_t &length() const;
            [[nodiscard]] const std::size_t &capacity() const;
            [[nodiscard]] const VALUE &get_value(const KEY &__k) const;
            [[nodiscard]] VALUE &get_value(const KEY &__k);

            [[nodiscard]] bool contains(const KEY &__k) const;

            [[nodiscard]] hashtable &operator=(const hashtable &ht);
            [[nodiscard]] hashtable &operator=(hashtable &&ht) noexcept(true);

            [[nodiscard]] const VALUE &operator[](const KEY &__k) const;
            [[nodiscard]] VALUE &operator[](const KEY &__k);

            [[nodiscard]] bool operator==(const hashtable &ht) const;
            [[nodiscard]] bool operator!=(const hashtable &ht) const;

            ~hashtable();
        };

        template <typename KEY, typename VALUE, typename hashing_function>
        void hashtable<KEY, VALUE, hashing_function>::init_map(const std::size_t &__c)
        {
            this->M_cap = __c;
            this->M_len = 0;
            this->M_table = new node<KEY, VALUE> *[this->M_cap]();
            horizon_misc::exit_heap_fail(this->M_table, "horizon::horizon_deps::hashtable");
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        void hashtable<KEY, VALUE, hashing_function>::rehash()
        {
            std::size_t new_cap = this->M_cap * 2;
            node<KEY, VALUE> **temp_node = new node<KEY, VALUE> *[new_cap]();
            horizon_misc::exit_heap_fail(temp_node, "horizon::horizon_deps::hashtable");
            for (std::size_t i = 0; i < this->M_cap; i++)
            {
                node<KEY, VALUE> *curr = this->M_table[i];
                while (curr)
                {
                    std::size_t index = hashtable::get_hash(curr->M_element.get_first(), new_cap);
                    node<KEY, VALUE> *next = curr->M_next;
                    curr->M_next = temp_node[index];
                    temp_node[index] = curr;
                    curr = next;
                }
            }
            delete[] this->M_table;
            this->M_table = temp_node;
            this->M_cap = new_cap;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        std::size_t hashtable<KEY, VALUE, hashing_function>::get_hash(const KEY &__k, const std::size_t &__c)
        {
            return hashing_function()(__k) % __c;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        hashtable<KEY, VALUE, hashing_function>::hashtable()
        {
            this->M_len = 0;
            this->M_cap = 0;
            this->M_table = nullptr;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        hashtable<KEY, VALUE, hashing_function>::hashtable(const std::size_t &init_cap)
        {
            this->init_map(init_cap);
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        hashtable<KEY, VALUE, hashing_function>::hashtable(const hashtable &ht)
        {
            this->init_map(ht.M_cap);
            for (std::size_t i = 0; i < ht.M_cap; i++)
            {
                node<KEY, VALUE> *curr = ht.M_table[i];
                while (curr)
                {
                    this->append(ht.M_table[i]->M_element);
                    curr = curr->M_next;
                }
            }
            this->M_len = ht.M_len;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        hashtable<KEY, VALUE, hashing_function>::hashtable(hashtable &&ht) noexcept(true)
        {
            this->M_len = ht.M_len;
            this->M_cap = ht.M_cap;
            this->M_table = ht.M_table;

            ht.M_len = 0;
            ht.M_cap = 0;
            ht.M_table = nullptr;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        bool hashtable<KEY, VALUE, hashing_function>::append(const KEY &__k, const VALUE &__v)
        {
            if (!this->M_table)
                this->init_map(16);
            std::size_t index = hashtable::get_hash(__k, this->M_cap);
            node<KEY, VALUE> *curr = this->M_table[index];
            while (curr)
            {
                if (curr->M_element.get_first() == __k)
                    return false;
                curr = curr->next;
            }
            node<KEY, VALUE> *temp = new node<KEY, VALUE>({__k, __v});
            horizon_misc::exit_heap_fail(temp, "horizon::horizon_deps::hashtable");
            temp->M_next = this->M_table[index];
            this->M_table[index] = temp;
            this->M_len++;
            if (this->M_len >= this->M_cap)
                this->rehash();
            return true;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        bool hashtable<KEY, VALUE, hashing_function>::append(KEY &&__k, VALUE &&__v)
        {
            if (!this->M_table)
                this->init_map(16);
            std::size_t index = hashtable::get_hash(__k, this->M_cap);
            node<KEY, VALUE> *curr = this->M_table[index];
            while (curr)
            {
                if (curr->M_element.get_first() == __k)
                    return false;
                curr = curr->M_next;
            }
            node<KEY, VALUE> *temp = new node<KEY, VALUE>({std::move(__k), std::move(__v)});
            horizon_misc::exit_heap_fail(temp, "horizon::horizon_deps::hashtable");
            temp->M_next = this->M_table[index];
            this->M_table[index] = temp;
            this->M_len++;
            if (this->M_len >= this->M_cap)
                this->rehash();
            return true;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        bool hashtable<KEY, VALUE, hashing_function>::append(const pair<KEY, VALUE> &__p)
        {
            if (!this->M_table)
                this->init_map(16);
            std::size_t index = hashtable::get_hash(__p.get_first(), this->M_cap);
            node<KEY, VALUE> *curr = this->M_table[index];
            while (curr)
            {
                if (curr->M_element.get_first() == __p.get_first())
                    return false;
                curr = curr->next;
            }
            node<KEY, VALUE> *temp = new node<KEY, VALUE>(__p);
            horizon_misc::exit_heap_fail(temp, "horizon::horizon_deps::hashtable");
            temp->M_next = this->M_table[index];
            this->M_table[index] = temp;
            this->M_len++;
            if (this->M_len >= this->M_cap)
                this->rehash();
            return true;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        bool hashtable<KEY, VALUE, hashing_function>::append(pair<KEY, VALUE> &&__p)
        {
            if (!this->M_table)
                this->init_map(16);
            std::size_t index = hashtable::get_hash(__p.get_first(), this->M_cap);
            node<KEY, VALUE> *curr = this->M_table[index];
            while (curr)
            {
                if (curr->M_element.get_first() == __p.get_first())
                    return false;
                curr = curr->next;
            }
            node<KEY, VALUE> *temp = new node<KEY, VALUE>(std::move(__p));
            horizon_misc::exit_heap_fail(temp, "horizon::horizon_deps::hashtable");
            temp->M_next = this->M_table[index];
            this->M_table[index] = temp;
            this->M_len++;
            if (this->M_len >= this->M_cap)
                this->rehash();
            return true;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        bool hashtable<KEY, VALUE, hashing_function>::remove(const KEY &__k)
        {
            std::size_t index = hashtable::get_hash(__k, this->M_cap);
            node<KEY, VALUE> *curr = this->M_table[index];
            node<KEY, VALUE> *prev = nullptr;
            while (curr)
            {
                if (curr->M_element.get_first() == __k)
                {
                    if (prev == nullptr)
                        this->table[hash] = curr->next;
                    else
                        prev->next = curr->next;
                    delete curr;
                    this->M_len--;
                    return true;
                }
                prev = curr;
                curr = curr->M_next;
            }
            return false;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        bool hashtable<KEY, VALUE, hashing_function>::is_null() const
        {
            return this->M_table == nullptr;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        bool hashtable<KEY, VALUE, hashing_function>::is_empty() const
        {
            return this->M_len == 0;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        bool hashtable<KEY, VALUE, hashing_function>::compare(const hashtable &ht) const
        {
            if (this->M_len != ht.M_len)
                return false;
            if (this->M_table == ht.M_table) // both point to same memory location
                return true;
            for (std::size_t i = 0; i < this->M_cap; i++)
            {
                node<KEY, VALUE> *curr = this->M_table[i];
                node<KEY, VALUE> *curr_other = ht.M_table[i];

                while (curr && curr_other)
                {
                    if (curr->M_element.get_first() != curr_other->M_element.get_first() || curr->M_element.get_second() != curr_other->M_element.get_second())
                        return false;
                    curr = curr->M_next;
                    curr_other = curr_other->M_next;
                }
            }
            return true;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        hashtable<KEY, VALUE, hashing_function> &hashtable<KEY, VALUE, hashing_function>::erase()
        {
            for (std::size_t i = 0; i < this->M_cap; i++)
            {
                node<KEY, VALUE> *curr = this->M_table[i];
                while (curr)
                {
                    node<KEY, VALUE> *next = curr->M_next;
                    delete curr;
                    curr = next;
                }
            }
            delete[] this->M_table;
            this->M_cap = 0;
            this->M_len = 0;
            this->M_table = nullptr;
            return *this;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        const std::size_t &hashtable<KEY, VALUE, hashing_function>::length() const
        {
            return this->M_len;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        const std::size_t &hashtable<KEY, VALUE, hashing_function>::capacity() const
        {
            return this->M_cap;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        const VALUE &hashtable<KEY, VALUE, hashing_function>::get_value(const KEY &__k) const
        {
            std::size_t index = hashtable::get_hash(__k, this->M_cap);
            node<KEY, VALUE> *curr = this->M_table[index];
            while (curr)
            {
                if (curr->M_element.get_first() == __k)
                    return curr->M_element.get_second();
                curr = curr->M_next;
            }
            if (COLOR_ERR)
                std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " " ENCLOSE(WHITE_FG, "horizon::horizon_deps::hashtable:") " key was not found in hashtable %p\n", static_cast<void *>(this->M_table));
            else
                std::fprintf(stderr, "horizon: error: horizon::horizon_deps::hashtable: key was not found in hashtable %p\n", static_cast<void *>(this->M_table));
            std::exit(EXIT_FAILURE);
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        VALUE &hashtable<KEY, VALUE, hashing_function>::get_value(const KEY &__k)
        {
            std::size_t index = hashtable::get_hash(__k, this->M_cap);
            node<KEY, VALUE> *curr = this->M_table[index];
            while (curr)
            {
                if (curr->M_element.get_first() == __k)
                    return curr->M_element.get_second();
                curr = curr->M_next;
            }
            if (COLOR_ERR)
                std::fprintf(stderr, "horizon: " ENCLOSE(RED_FG, "error:") " " ENCLOSE(WHITE_FG, "horizon::horizon_deps::hashtable:") " key was not found in hashtable %p\n", static_cast<void *>(this->M_table));
            else
                std::fprintf(stderr, "horizon: error: horizon::horizon_deps::hashtable: key was not found in hashtable %p\n", static_cast<void *>(this->M_table));
            std::exit(EXIT_FAILURE);
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        bool hashtable<KEY, VALUE, hashing_function>::contains(const KEY &__k) const
        {
            std::size_t index = hashtable::get_hash(__k, this->M_cap);
            node<KEY, VALUE> *curr = this->M_table[index];
            while (curr)
            {
                if (curr->M_element.get_first() == __k)
                    return true;
                curr = curr->M_next;
            }
            return false;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        hashtable<KEY, VALUE, hashing_function> &hashtable<KEY, VALUE, hashing_function>::operator=(const hashtable &ht)
        {
            if (this != &ht)
            {
                this->erase();
                this->init_map(ht.M_cap);
                for (std::size_t i = 0; i < ht.M_cap; i++)
                {
                    node<KEY, VALUE> *curr = ht.M_table[i];
                    while (curr)
                    {
                        this->append(ht.M_table[i]->M_element);
                        curr = curr->M_next;
                    }
                }
            }
            return *this;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        hashtable<KEY, VALUE, hashing_function> &hashtable<KEY, VALUE, hashing_function>::operator=(hashtable &&ht) noexcept(true)
        {
            if (this != &ht)
            {
                this->erase();
                this->M_len = ht.M_len;
                this->M_cap = ht.M_cap;
                this->M_table = ht.M_table;

                ht.M_len = 0;
                ht.M_cap = 0;
                ht.M_table = nullptr;
            }
            return *this;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        const VALUE &hashtable<KEY, VALUE, hashing_function>::operator[](const KEY &__k) const
        {
            return this->get_value(__k);
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        VALUE &hashtable<KEY, VALUE, hashing_function>::operator[](const KEY &__k)
        {
            return this->get_value(__k);
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        bool hashtable<KEY, VALUE, hashing_function>::operator==(const hashtable &ht) const
        {
            return this->compare(ht);
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        bool hashtable<KEY, VALUE, hashing_function>::operator!=(const hashtable &ht) const
        {
            return !this->compare(ht);
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        hashtable<KEY, VALUE, hashing_function>::~hashtable()
        {
            this->erase();
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        class hashtable_iter
        {
          private:
            hashtable<KEY, VALUE, hashing_function> *M_hashtable;
            node<KEY, VALUE> *M_curr;
            std::size_t M_index;

            hashtable_iter(hashtable<KEY, VALUE, hashing_function> *map);
            const pair<KEY, VALUE> &operator->() const;
            pair<KEY, VALUE> &operator->();
            hashtable_iter &next();
            hashtable_iter &operator++();
        };

        template <typename KEY, typename VALUE, typename hashing_function>
        hashtable_iter<KEY, VALUE, hashing_function>::hashtable_iter(hashtable<KEY, VALUE, hashing_function> *map)
        {
            this->M_hashtable = map;
            this->M_curr = nullptr;
            this->M_index = 0;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        const pair<KEY, VALUE> &hashtable_iter<KEY, VALUE, hashing_function>::operator->() const
        {
            return this->M_curr->M_element;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        pair<KEY, VALUE> &hashtable_iter<KEY, VALUE, hashing_function>::operator->()
        {
            return this->M_curr->M_element;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        hashtable_iter<KEY, VALUE, hashing_function> &hashtable_iter<KEY, VALUE, hashing_function>::next()
        {
            if (!this->M_curr)
            {
                this->M_curr = this->M_hashtable->M_table[this->M_index];
                while (this->M_curr == nullptr)
                {
                    this->M_curr = this->M_hashtable->M_table[this->M_index++];
                }
            }
            else if (this->M_curr->M_next)
            {
                this->M_curr = this->M_curr->M_next;
            }
            else
                this->M_index++;
            return *this;
        }

        template <typename KEY, typename VALUE, typename hashing_function>
        hashtable_iter<KEY, VALUE, hashing_function> &hashtable_iter<KEY, VALUE, hashing_function>::operator++()
        {
            this->next();
        }
    }
}

#endif