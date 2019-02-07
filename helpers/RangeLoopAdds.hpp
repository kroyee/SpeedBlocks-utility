#ifndef RANGELOOPADDS_HPP
#define RANGELOOPADDS_HPP

#include <utility>

template <class Container>
class LoopIndex {
   public:
    LoopIndex(Container& container) : m_container(container) {}

    using base_iterator = typename Container::iterator;
    using base_value_type = typename base_iterator::value_type;

    struct iterator {
        iterator(base_iterator iter) : iter_index_pair(std::move(iter), 0) {}

        using internal_value_type = std::pair<base_iterator, std::size_t>;
        using value_type = std::pair<base_value_type&, std::size_t>;

        struct Proxy {
            Proxy(iterator& iter) : value(*iter.iter_index_pair.first, iter.iter_index_pair.second) {}
            value_type* operator->() { return &value; }

           private:
            value_type value;
        };

        using reference = value_type;
        using pointer = Proxy;
        using difference_type = typename base_iterator::difference_type;

        iterator& operator++() {
            ++iter_index_pair.first;
            ++iter_index_pair.second;
            return *this;
        }
        iterator operator++(int) {
            auto tmp = iter_index_pair;
            ++(*this);
            return tmp;
        }
        reference operator*() { return {*iter_index_pair.first, iter_index_pair.second}; }
        pointer operator->() { return {*this}; }
        bool operator==(const iterator& other) { return iter_index_pair.first == other.iter_index_pair.first; }
        bool operator!=(const iterator& other) { return !(*this == other); }

       private:
        internal_value_type iter_index_pair;
    };

    iterator begin() { return iterator(m_container.begin()); }
    iterator end() { return iterator(m_container.end()); }

   private:
    Container& m_container;
};

template <class Container>
class LoopIter {
   public:
    LoopIter(Container& container) : m_container(container) {}

    using base_iterator = typename Container::iterator;
    using base_value_type = typename base_iterator::value_type;

    struct iterator {
        iterator(base_iterator iter) : m_iter(std::move(iter)) {}

        using internal_value_type = base_iterator;
        using value_type = base_iterator;
        using reference = value_type&;
        using pointer = value_type*;
        using difference_type = typename base_iterator::difference_type;

        iterator& operator++() {
            ++m_iter;
            return *this;
        }
        iterator operator++(int) {
            auto tmp = m_iter;
            ++(*this);
            return tmp;
        }
        reference operator*() { return m_iter; }
        pointer operator->() { return &m_iter; }
        bool operator==(const iterator& other) { return m_iter == other.m_iter; }
        bool operator!=(const iterator& other) { return !(*this == other); }

       private:
        internal_value_type m_iter;
    };

    iterator begin() { return iterator(m_container.begin()); }
    iterator end() { return iterator(m_container.end()); }

   private:
    Container& m_container;
};

#endif