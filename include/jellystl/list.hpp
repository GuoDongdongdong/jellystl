#ifndef JELLY_LIST
#define JELLY_LIST

#include<initializer_list>

#include"allocator.hpp"

namespace jelly {


namespace list_detail {

struct list_node_base {

    list_node_base* next;
    list_node_base* prev;

    list_node_base(): next(this), prev(this) {}
    
    list_node_base(list_node_base* _next, list_node_base* _prev): next(_next), prev(_prev) {}

    list_node_base(list_node_base&& other){
        next = other.next;
        prev = other.prev;
        other.next = nullptr;
        other.prev = nullptr;
    }
};

template<typename T>
struct list_node : public list_node_base{

    T data;
    list_node(): list_node_base(), data(){}


    template<typename... Args>
    list_node(list_node_base* next, list_node_base* prev, Args&&... args): list_node_base(next, prev), data(std::forward<Args>(args)...) {}
};

struct list_header : public list_node_base {
    
    std::size_t _size;
    
    list_header(): list_node_base(), _size() {}

    list_header(const list_header& ) = delete;

    list_header(list_header&& other):list_node_base(other.next, other.prev), _size(other.size()) {
        if(other.next == other.prev){
            next = prev = this;
        }
        else{
            next -> prev = prev -> next = this;
            other.next = other.prev = &other;
            other._size = 0;
        }
    }

    std::size_t size() const noexcept {
        return _size;
    }

    void inc_size(std::size_t inc) noexcept {
        _size += inc;
    }

    void dec_size(std::size_t dec) noexcept {
        _size -= dec;
    }
};

template<typename T>
struct list_iterator{
    using iterator_category = bidirectional_iterator_tag;
    using value_type = T;
    using distance_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    using self = list_iterator;
    using Node = list_node<T>;

    list_node_base* node;

    list_iterator() noexcept : node() {}

    list_iterator(list_node_base* node_base) noexcept : node(node_base) {}

    reference operator*() const noexcept {
        return (static_cast<Node*>(node)) -> data;
    }

    pointer operator->() const noexcept {
        return &(static_cast<Node*>(node) -> data);
    }

    self& operator++() noexcept {
        node = node -> next;
        return *this;
    }

    self operator++(int) noexcept {
        self tmp = *this;
        node = node -> next;
        return tmp;
    }

    self& operator--() noexcept {
        node = node -> prev;
        return *this;
    }

    self operator--(int) noexcept {
        self tmp = *this;
        node = node -> prev;
        return tmp;
    }

    friend bool operator==(const self& x, const self& y) noexcept {
        return x.node == y.node;
    }

    friend bool operator!=(const self& x, const self& y) noexcept {
        return !(x.node == y.node);
    }

};

template<typename T>
struct list_const_iterator{
    using iterator_category = bidirectional_iterator_tag;
    using value_type = T;
    using distance_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    using self = list_const_iterator;
    using Node = list_node<T>;

    const list_node_base* node;

    list_const_iterator() noexcept : node() {}

    list_const_iterator(const list_node_base* node_base) noexcept : node(node_base) {}

    list_const_iterator(const list_iterator<T>& iterator) noexcept : node(iterator.node) {}

    list_iterator<T> _const_cast() const noexcept {
        return list_iterator<T>(const_cast<list_node_base*>(node));
    }

    reference operator*() const noexcept {
        return (static_cast<const Node*>(node)) -> data;
    }

    pointer operator->() const noexcept {
        return static_cast<const Node *>(node);
    }

    self& operator++() noexcept {
        node = node -> next;
        return *this;
    }

    self operator++(int) noexcept {
        self tmp = *this;
        node = node -> next;
        return tmp;
    }

    self& operator--() noexcept {
        node = node -> prev;
        return *this;
    }

    self operator--(int) noexcept {
        self tmp = *this;
        node = node -> prev;
        return tmp;
    }

    friend bool operator==(const self& x, const self& y) noexcept {
        return x.node == y.node;
    }

    friend bool operator!=(const self& x, const self& y) noexcept {
        return !(x.node == y.node);
    }

};

} 

template<typename T, typename allo = allocator<T> >
class list {
public:
    using value_type = T;
	using pointer = T*;
    using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using size_type = std::size_t;
    using distance_type = std::ptrdiff_t;
	using iterator = list_detail::list_iterator<T>;
	using const_iterator = list_detail::list_const_iterator<T>;

private:
    using node = list_detail::list_node<T>;
    using node_allo = typename allo::template rebind<node>::other;

    list_detail::list_header header;

    // position 不属于 [first, last) 
    void transfer(const_iterator position, const_iterator first, const_iterator last) {
        iterator _position = position._const_cast();
        iterator _first = first._const_cast();
        iterator _last = last._const_cast();
        if(_position == _last) {
            return ;
        }
        _position.node -> prev -> next = _first.node;
        _first.node -> prev -> next = _last.node;
        _last.node -> prev -> next = _position.node;
        auto tmp = _position.node -> prev;
        _position.node -> prev = _last.node -> prev;
        _last.node -> prev = _first.node -> prev;
        _first.node -> prev = tmp;
    }

public:
    list(): header() {}
    
    list(size_type n) {
        while(n--) {
            emplace(cend());
        }
    }
    
    list(size_type n, const T& value) {
        while(n--) {
            emplace(cend(), value);
        }
    }
    
    template<typename InputIterator>
    list(InputIterator first, InputIterator last) {
        for(; first != last; ++first){
            emplace(cend(), *first);
        }
    }

    list(std::initializer_list<T> _list): list(_list.begin(), _list.end()) {}

    list(const list& other): list(other.cbegin(), other.cend()) {}

    list(list&& other) noexcept : header(std::move(other.header)) {}

    ~list() {
        erase(cbegin(), cend());
    }

    list& operator=(const list& other) {
        if(this == &other) return *this;
        iterator cur_this = begin();
        const_iterator cur_other = other.cbegin();
        while(cur_this != end() && cur_other != other.cend()) {
            *cur_this = *cur_other;
            ++cur_this;
            ++cur_other;
        }
        while(cur_this != end()) {
            cur_this = erase(cur_this);
        }
        while(cur_other != other.cend()) {
            insert(cend(), *cur_other);
        }
        return *this;
    }

    list& operator=(list&& other) noexcept {
        if(this == &other) return *this;
        clear();
        if(!other.empty()) {
            header.next = other.header.next;
            header.prev = other.header.prev;
            header._size = other.size();
            other.header.next -> prev = other.header.prev -> next = &header;
            other.header.next = other.header.prev = &(other.header);
            other.header._size = 0;
        }
        return *this;
    }
    
    list& operator=(std::initializer_list<T> other) {
        iterator cur_this = begin();
        auto cur_other = other.begin();
        while(cur_this != end() && cur_other != other.end()) {
            *cur_this = *cur_other;
            ++cur_this;
            ++cur_other;
        }
        while(cur_this != end()) {
            erase(cur_this);
        }
        while(cur_other != other.end()) {
            insert(cur_this, *cur_other);
            ++cur_other;
        }
        return *this;
    }

    iterator begin() noexcept {
        return header.next;
    }

    const_iterator begin() const noexcept {
        return static_cast<const_iterator>(header.next);
    }

    const_iterator cbegin() const noexcept {
        return static_cast<const_iterator>(header.next);
    }

    iterator end(){
        return &header;
    }
    
    const_iterator end() const noexcept {
        return static_cast<const_iterator>(&header);
    }

    const_iterator cend() const {
        return static_cast<const_iterator>(&header);
    }
    
    size_type size() const noexcept {
        return header.size();
    }

    bool empty() const {
        return cbegin() == cend();
    }

    void swap(list& other) {
        if(other.empty() && empty()) {
            return ;
        } else if(other.empty()) {
            other.header.next = header.next;
            other.header.prev = header.prev;
            header.next -> prev = header.prev -> next = &(other.header);
            header.next = header.prev = &header;
        } else if(empty()) {
            header.next = other.header.next;
            header.prev = other.header.prev;
            other.header.next -> prev = other.header.prev -> next = &header;
            other.header.next = other.header.prev = &(other.header);
        } else {
            auto p = header.next;
            header.next -> prev = &(other.header);
            header.next = other.header.next;
            other.header.next -> prev = &header;
            other.header.next = p;
            
            p = header.prev;
            header.prev -> next = &(other.header);
            header.prev = other.header.prev;
            other.header.prev -> next = &header;
            other.header.prev = p;
        }
        std::swap(header._size, other.header._size);
    }

    reference front() {
        return *begin();
    }

    const_reference front() const {
        return *cbegin();
    }

    reference back(){
        return static_cast<node*>(header.prev) -> data;
    }

    const_reference back() const {
        return static_cast<const node*>(header.prev) -> data;
    }

    template<typename U>
    void push_front(U&& value){
        insert(begin(), std::forward<U>(value));
    }

    template<typename U>
    void push_back(U&& value){
        insert(end(), std::forward<U>(value));
    }

    void pop_front(){
        erase(cbegin());
    }

    void pop_back() {
        erase(header.prev);
    }

    template<typename... Args>
    iterator emplace(const_iterator position, Args&&... args) {
        iterator _position = position._const_cast();
        node* p = node_allo::allocate(1);
        try {
            construct(p, _position.node, _position.node -> prev, std::forward<Args>(args)...);
        } catch(...) {
            node_allo::deallocate(p);
            throw;
        }
        _position.node -> prev -> next = p;
        _position.node -> prev = p;
        header.inc_size(1);
        return iterator(p);
    }

    iterator insert(const_iterator position, T&& value) {
        return emplace(position, std::move(value));
    }
    
    iterator insert(const_iterator position, const T& value) {
        return emplace(position, value);
    }

    iterator insert(const_iterator position, const T& value, size_type n) {
        iterator ret = position._const_cast();
        while(n--) {
            ret = insert(position, value);
        }
        return ret;
    }

    template<typename InputIterator>
    iterator insert(const_iterator position, InputIterator first, InputIterator last) {
        iterator ret = position._const_cast();
        for(; first != last; ++first) {
            ret = insert(position, *first);
        }
        return ret;
    }

    iterator insert(const_iterator position, std::initializer_list<T> list) {
        return insert(position, list.begin(), list.end());
    }

    iterator erase(const_iterator position) {
        iterator _position = position._const_cast();
        iterator ret = _position.node -> next;
        _position.node -> prev -> next = _position.node -> next;
        _position.node -> next -> prev = _position.node -> prev;
        node* p = static_cast<node*>(_position.node);
        destory(p);
        node_allo::deallocate(p);
        header.dec_size(1);
        return ret;
    }

    iterator erase(const_iterator first, const_iterator last) {
        iterator _first = first._const_cast();
        iterator _last = last._const_cast();
        while(_first != last) {
            _first = erase(_first);
        }
        return _last;
    }

    void clear() {
        erase(cbegin(), cend());
    }
    
    // 两个list必须有序
    void merge(list&& other) {
        iterator first_this = begin();
        iterator last_this = end();
        iterator first_other = other.begin();
        iterator last_other = other.end();
        while(first_this != last_this && first_other != last_other) {
            if((*first_other) < (*first_this)) {
                iterator next = first_other;
                ++next;
                transfer(first_this, first_other, next);
                first_other = next;
                header.inc_size(1);
                other.header.dec_size(1);
            }
            else{
                ++first_this;
            }
        }
        if(!other.empty()) {
            const size_type add_size = other.size();
            transfer(end(), other.begin(), other.end());
            header.inc_size(add_size);
            other.header._size = 0;
        }

    }

    void merge(list& other) {
        merge(std::move(other));
    }

    void splice(const_iterator position, list& other, const_iterator first, const_iterator last) {
        if(position == last) {
            return ;
        }
        const size_type n = Distance(first, last);
        transfer(position, first, last);
        header.inc_size(n);
        other.header.inc_size(n);
    }

    void splice(const_iterator position, list& other, const_iterator _i) {
        const_iterator next = _i;
        ++next;
        if(position == _i || position == next) {
            return ;
        }
        transfer(position, _i, next);
        header.inc_size(1);
        other.header.dec_size(1);
    }

    void sort() {
        if(size() == 0 || size() == 1) {
            return ;
        }
        list carry;
        list tmp[64];
        int fill = 0;
        while(!empty()) {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            for(; i < fill && !(tmp[i].empty()); ++i) {
                carry.merge(tmp[i]);
            }
            tmp[i].swap(carry);
            if(i == fill) {
                ++fill;
            }
        }
        for(int i = 0; i < fill; ++i) {
            carry.merge(tmp[i]);
        }
        swap(carry);
    }
};

template<typename T, typename Allo>
inline void swap(list<T, Allo>& lhs, list<T, Allo>& rhs) noexcept {
	lhs.swap(rhs);
}

}

#endif