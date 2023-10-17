#ifndef JELLY_RB_TREE
#define JELLY_RB_TREE

#include<cstddef> // std::size_t std::ptrdiff_t

#include"allocator.hpp"

namespace jelly {

namespace rb_tree_detail {
    enum Color {RED = 0, BLACK = 1};

    struct rb_tree_node_base {
        using self = rb_tree_node_base;
        Color color;
        self* left;
        self* right;
        self* parent;
    };

    template<typename T>
    struct rb_tree_node : public rb_tree_node_base {
        T val;
    };
    
    struct rb_tree_header {
        rb_tree_node_base header;
        std::size_t size;

    };
    
    template<typename T>
    struct rb_tree_iterator {

    };

    template<typename T>
    struct rb_tree_const_iterator {
        
    };
    
}

template<typename Key, typename Value, typename Compare, typename allo = allocator<Value> >
class rb_tree {
public:
    using value_type = Value;
    using pointer = Value*
    using const_pointer = const Value*;
    using reference = Value&;
    using const_reference = const Value&;
    using size_type = std::size_t;
    using distance_type = std::ptrdiff_t;
    using iterator = rb_tree_detail::rb_tree_iterator<T>;
    using const_iterator = rb_tree_detail::rb_tree_const_iterator<T>;
    using allocator_type = allo;

private:
    using key_type = Key;
    using node = rb_tree_detail::rb_tree_node;
    using node_allo = allo::template rebind<node>::other;
    
public:
    rb_tree();
    rb_tree(const self& other);
    rb_tree(self&& other);
    const self& operator=(const self& other);
    const self& operator=(self&& other);
    ~rb_tree();

    iterator begin() noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator cend() const noexcept;
    size_type size() const noexcept;
    bool empty() const noexcept;
    void clear();

    void swap(const self& other) noexcept;
};

template<typename Key, typename Value, typename Compare, typename allo>
inline void swap(rb_tree<Key, Value, Compare, allo>& lhs, rb_tree<Key, Value, Compare, allo>& rhs) {
    lhs.swap(rhs);
}

}

#endif