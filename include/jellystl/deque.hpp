#ifndef JELLY_DEQUE
#define JELLY_DEQUE

#include<initializer_list>
#include<type_traits> // is_same_v

#include"allocator.hpp"
#include"uninitialized.hpp"

namespace jelly {

namespace deque_detail {

#define DEQUE_NODE_SIZE 512

constexpr inline std::size_t deque_node_num(std::size_t T_size) {
    return (DEQUE_NODE_SIZE < T_size) ? 1 : (DEQUE_NODE_SIZE / T_size);
}

template<typename T, typename Ref, typename Ptr>
struct deque_iterator {
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using distance_type = std::ptrdiff_t;
    
    using iterator = deque_iterator<T, T&, T*>;
    using const_iterator = deque_iterator<T, const T&, const T*>;

    using pointer = Ptr;
    using reference = Ref;
    using map_pointer = T**;
    using self = deque_iterator;
    
    pointer first;
    pointer last;
    pointer cur;
    map_pointer node;
    
    void set_node(map_pointer _node, distance_type cur_offset) {
        first = *_node;
        last = (*_node) + deque_node_num(sizeof(T));
        cur = first + cur_offset;
        node = _node;
    }

    iterator _const_cast() {
        iterator ret = iterator();
        ret.set_node(node, cur - first);
        return ret;
    }

    deque_iterator(): first(), last(), cur(), node() {}

    template<typename Itr,
    typename = std::enable_if_t<std::is_same_v<Itr, iterator> && std::is_same_v<self, const_iterator> > >
    deque_iterator(const Itr& i): first(i.first), last(i.last), cur(i.cur), node(i.node) {}

    deque_iterator(const self&) = default;
    deque_iterator(self&&) = default;
    self& operator=(const self&) = default;
    self& operator=(self&&) = default;
    ~deque_iterator() = default;

    reference operator*() const noexcept {
        return *cur;
    }

    pointer operator->() const noexcept {
        return cur;
    }
    
    self& operator++() noexcept {
        ++cur;
        if(cur == last) {
            set_node(node + 1, 0);
        }
        return *this;
    }

    self operator++(int) noexcept {
        self ret = *this;
        ++(*this);
        return ret;
    }

    self& operator--() noexcept {
        if(cur == first) {
            set_node(node - 1, deque_node_num(sizeof(T)));
        }
        --cur;
        return *this;
    }

    self operator--(int) noexcept {
        self ret = *this;
        --(*this);
        return ret;
    }

    self& operator+=(distance_type n) noexcept {
        distance_type offset = n + (cur - first);
        if(offset >= 0 && offset < deque_node_num(sizeof(T))) {
            cur += n;
        } else {
            distance_type offset_node = offset > 0 ?  offset / deque_node_num(sizeof(T)) : -((-offset - 1) / deque_node_num(sizeof(T))) - 1;
            set_node(node + offset_node, offset - deque_node_num(sizeof(T)) * offset_node);
        }
        return *this;
    }

    self operator+(distance_type n) noexcept {
        self ret = *this;
        return ret += n;
    }

    self& operator-=(distance_type n) noexcept {
        return *this += -n;
    }

    self operator-(distance_type n) noexcept {
        self ret = *this;
        // cout << "operator-";
        // cout << node << ' ' << cur << endl;
        return ret += -n;
    }

    reference operator[](distance_type n) noexcept {
        return *(this + n);
    }
};

template<typename T, typename Ref, typename Ptr>
inline bool operator==(const deque_iterator<T, Ref, Ptr>& lhs, const deque_iterator<T, Ref, Ptr>& rhs) {
    return lhs.cur == rhs.cur;
}

template<typename T, typename Ref, typename Ptr>
inline bool operator!=(const deque_iterator<T, Ref, Ptr>& lhs, const deque_iterator<T, Ref, Ptr>& rhs) {
    return !(lhs.cur == rhs.cur);
}

template<typename T, typename Ref, typename Ptr>
inline bool operator<(const deque_iterator<T, Ref, Ptr>& lhs, const deque_iterator<T, Ref, Ptr>& rhs) {
    return (lhs.node == rhs.node) ? (lhs.cur < rhs.cur) : (lhs.node < rhs.node);
}

template<typename T, typename Ref, typename Ptr>
inline bool operator>(const deque_iterator<T, Ref, Ptr>& lhs, const deque_iterator<T, Ref, Ptr>& rhs) {
    return (lhs.node == rhs.node) ? (lhs.cur > rhs.cur) : (lhs.node > rhs.node);
}

template<typename T, typename Ref, typename Ptr>
inline bool operator<=(const deque_iterator<T, Ref, Ptr>& lhs, const deque_iterator<T, Ref, Ptr>& rhs) {
    return !(lhs > rhs);
}

template<typename T, typename Ref, typename Ptr>
inline bool operator>=(const deque_iterator<T, Ref, Ptr>& lhs, const deque_iterator<T, Ref, Ptr>& rhs) {
    return !(lhs < rhs);
}

template<typename T, typename Ref, typename Ptr>
inline std::ptrdiff_t operator-(const deque_iterator<T, Ref, Ptr>& lhs, const deque_iterator<T, Ref, Ptr>& rhs) {
    // cout << lhs.node << ' ' << rhs.node << endl;
    return (lhs.node - rhs.node - 1) * deque_node_num(sizeof(T)) + (lhs.cur - lhs.first) + (rhs.last - rhs.cur);
}
};


// start finish 指向 第一个元素和最后一个元素后一个位置（有效位置）
// start finish 以外的节点没有分配资源

template<typename T, typename allo = allocator<T> >
class deque {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using distance_type = std::ptrdiff_t;
    using iterator = deque_detail::deque_iterator<T, T&, T*>;
    using const_iterator = deque_detail::deque_iterator<T, const T&, const T*>;

private:
    using map_pointer = pointer*;
    using map_allo = allocator<pointer>;
    
    iterator start;
    iterator finish;
    map_pointer map;
    size_type map_size;

    void allocate_node_data(size_type n) {
        const size_type node_num = (n / deque_detail::deque_node_num(sizeof(T))) + 1;
        map_size = node_num + 2;
        map = map_allo::allocate(map_size);
        map_pointer map_start = map + 1;
        map_pointer map_finish = map_start + node_num;
        map_pointer cur = map_start;
        try {
            for(; cur != map_finish; ++cur){
                *cur = allo::allocate(deque_detail::deque_node_num(sizeof(T)));
            }
            start.set_node(map_start, 0);
            finish.set_node(map_finish - 1, n % deque_detail::deque_node_num(sizeof(T)));
        } catch(...) {
            for(map_pointer i = map_start; i != cur; ++i) {
                allo::deallocate(*i, deque_detail::deque_node_num(sizeof(T)));
            }
            map_allo::deallocate(map, map_size);
            map_size = 0;
            map = map_pointer();
            start = finish = iterator();
        }
    }

    void destory_data(const_iterator first, const_iterator last) {
        if(first.node != last.node) {
            destory(first.cur, first.last);
            for(map_pointer i = first.node + 1; i < last.node; ++i) {
                destory((*i), (*i) + deque_detail::deque_node_num(sizeof(T)));
            }
            destory(last.first, last.cur);
        } else {
            destory(first.cur, last.cur);
        }
    }

    void destory_node(map_pointer first, map_pointer last) {
        for(map_pointer i = first; i < last; ++i) {
            allo::deallocate(*i, deque_detail::deque_node_num(sizeof(T)));
        }
    }

    void destory_map() {
        map_allo::deallocate(map, map_size);
    }
    
    // 在map头部或尾部加入节点，start和finish不变。
    void reallocate_map(size_type node_add, bool front) {
        const size_type old_node_size = finish.node - start.node + 1;
        const size_type new_node_size = old_node_size + node_add;
        map_pointer new_start;
        if(map_size < new_node_size) {
            const size_type new_map_size = new_node_size + 2;
            map_pointer new_map = map_allo::allocate(new_map_size);
            new_start = new_map + 1 + (front ? node_add : 0);
            copy(start.node, finish.node + 1, new_start);
            map_allo::deallocate(map, map_size);
            map = new_map;
            map_size = new_map_size;
        } else {
            new_start = map + (front ? node_add : 0);
            if(new_start < start.node) {
                copy(start.node, finish.node + 1, new_start);
            } else {
                backward_copy(start.node, finish.node + 1, new_start + old_node_size);
            }
        }
        start.node = new_start;
        finish.node = new_start + old_node_size - 1;
    }

    void resever_map_at_back(size_type node_add = 1) {
        if(map_size - (finish.node - map + 1) >= node_add) return;
        reallocate_map(node_add, false);
    }

    void resever_map_at_front(size_type node_add = 1) {
        if(start.node - map >= node_add) return;
        reallocate_map(node_add, true);
    }

    void erase_to_end(const_iterator position) {
        destory_data(position, cend());
        destory_node(position.node + 1, finish.node + 1);
        finish = position._const_cast();
    }

    void erase_to_begin(const_iterator position) {
        destory_data(begin(), position);
        destory_node(start.node, position.node);
        start = position._const_cast();
    }

    // 在首部分配n个元素
    void new_element_at_front(size_type n) {
        const size_type node_add = (n + deque_detail::deque_node_num(sizeof(T)) - 1) / deque_detail::deque_node_num(sizeof(T));
        resever_map_at_front(node_add);
        size_type i = 1;
        try {
            for(; i <= node_add; ++i) {
                *(start.node - i) = allo::allocate(deque_detail::deque_node_num(sizeof(T)));
            }
        } catch(...) {
            for(size_type j = 1; j < i; ++j) {
                allo::deallocate(*(start.node - j), deque_detail::deque_node_num(sizeof(T)));
            }
        }
    }

    // 在首部保留n个元素
    iterator resever_element_at_front(size_type n) {
        const size_type vacancies = start.cur - start.first;
        if(n > vacancies) {
            new_element_at_front(n);
        }
        return start - n;
    }

    // 在尾部分配n个元素
    void new_element_at_back(size_type n) {
        const size_type node_add = (n + deque_detail::deque_node_num(sizeof(T)) - 1) / deque_detail::deque_node_num(sizeof(T));
        resever_map_at_back(node_add);
        size_type i = 1;
        try {
            for(; i <= node_add; ++i) {
                *(finish.node + i) = allo::allocate(deque_detail::deque_node_num(sizeof(T)));
            }
        } catch(...) {
            for(size_type j = 1; j < i; ++j) {
                allo::deallocate(*(finish.node + j), deque_detail::deque_node_num(sizeof(T)));
            }
            throw;
        }
    }

    // 在尾部保留n个元素
    iterator resever_element_at_back(size_type n) {
        // 留一个空缺给finish用
        const size_type vacancies = finish.last - finish.cur - 1;
        if(n > vacancies) {
            new_element_at_back(n - vacancies);
        }
        return finish + n;
    }

    iterator fill_insert(const_iterator position, size_type n, const T& value) {
        iterator _position = position._const_cast();
        const size_type element_before = _position - begin();
        if(_position.cur == start.cur) { // 在头部插入
            iterator new_start = resever_element_at_front(n);
            try {
                uninitialized_fill_n(new_start, n, value);
                start = new_start;
            } catch(...) {
                destory_node(new_start.node, start.node);
            }
        } else if(_position.cur == finish.cur) { // 在尾部插入
            iterator new_finish = resever_element_at_back(n);
            try {
                uninitialized_fill_n(finish, n, value);
                finish = new_finish;
            } catch(...) {
                destory_node(finish.node + 1, new_finish.node + 1);
            }
        } else { // 在中间插入
            if(element_before < size() / 2) {
                iterator new_start = resever_element_at_front(n);
                // resever_element_at_front可能导致map重新分配 _position迭代器失效
                _position = begin() + element_before;
                try { // 异常抛出时，deque不再可用
                    if(element_before >= n) {
                        uninitialized_move(begin(), begin() + n, new_start);
                        move(begin() + n, _position, new_start + n);
                        fill(_position - n, _position, value);
                    } else {
                        uninitialized_move(begin(), _position, new_start);
                        uninitialized_fill_n(new_start + element_before, n - element_before, value);
                        fill(begin(), _position, value);
                    }
                    start = new_start;
                } catch(...) {
                    destory_node(new_start.node, start.node + 1);
                    throw;
                }
            } else {
                const size_type element_after = end() - _position;
                iterator new_finish = resever_element_at_back(n);
                // resever_element_at_front可能导致map重新分配 _position迭代器失效
                _position = begin() + element_before;
                try {
                    if(element_after >= n) {
                        uninitialized_move(end() - n, end(), finish);
                        backward_move(_position, end() - n, finish);
                        fill(_position, _position + n, value);
                    } else {
                        uninitialized_move(_position, end(), new_finish - element_after);
                        fill(_position, _position + element_after, value);
                        uninitialized_fill_n(finish, n - element_after, value);
                    }
                    finish = new_finish;
                } catch(...) {
                    destory_node(finish.node + 1, new_finish.node + 1);
                    throw;
                }
            }
        }
        return begin() + element_before;
    }

    template<typename InputIterator>
    iterator range_insert(const_iterator position, InputIterator first, InputIterator last) {
        iterator _position = position._const_cast();
        const size_type element_before = _position - begin();
        const size_type n = Distance(first, last);
        if(_position.cur == start.cur) { // 在头部插入
            iterator new_start = resever_element_at_front(n);
            try {
                uninitialized_copy(first, last, new_start);
                start = new_start;
            } catch(...) {
                destory_node(new_start.node, start.node);
            }
        } else if(_position.cur == finish.cur) { // 在尾部插入
            iterator new_finish = resever_element_at_back(n);
            try {
                uninitialized_copy(first, last, finish);
                finish = new_finish;
            } catch(...) {
                destory_node(finish.node + 1, new_finish.node + 1);
            }
        } else { // 在中间插入
            if(element_before < size() / 2) {
                iterator new_start = resever_element_at_front(n);
                // resever_element_at_front可能导致map重新分配 _position迭代器失效
                _position = begin() + element_before;
                try { // 异常抛出时，deque不再可用
                    if(element_before >= n) {
                        uninitialized_move(begin(), begin() + n, new_start);
                        move(begin() + n, _position, new_start + n);
                        copy(first, last, _position - n);
                    } else {
                        InputIterator mid = first;
                        Advance(mid, n - element_before);
                        uninitialized_move(begin(), _position, new_start);
                        uninitialized_copy(first, mid, new_start + element_before);
                        copy(mid, last, begin());
                    }
                    start = new_start;
                } catch(...) {
                    destory_node(new_start.node, start.node + 1);
                    throw;
                }
            } else {
                const size_type element_after = end() - _position;
                iterator new_finish = resever_element_at_back(n);
                // resever_element_at_front可能导致map重新分配 _position迭代器失效
                _position = begin() + element_before;
                try {
                    if(element_after >= n) {
                        uninitialized_move(end() - n, end(), finish);
                        backward_move(_position, end() - n, finish);
                        copy(first, last, _position);
                    } else {
                        InputIterator mid = first;
                        Advance(mid, element_after);
                        uninitialized_move(_position, end(), new_finish - element_after);
                        copy(first, mid, _position);
                        uninitialized_copy(mid, last, finish);
                    }
                    finish = new_finish;
                } catch(...) {
                    destory_node(finish.node + 1, new_finish.node + 1);
                    throw;
                }
            }
        }
        return begin() + element_before;
    }

public:
    deque(): start(), finish(), map_size(0), map(){
        allocate_node_data(0);
    }

    deque(size_type n) {
        allocate_node_data(n);
        map_pointer cur = start.node;
        try {
            for(; cur != finish.node; ++cur) {
                uninitialized_default_n(*cur, deque_detail::deque_node_num(sizeof(T)));
            }
            uninitialized_default_n(*(finish.node), (finish.cur - finish.first));
        } catch(...) {
            for(map_pointer i = cur; i != cur; ++i) {
                destory(*i, (*i) + deque_detail::deque_node_num(sizeof(T)));
            }
        }
    }

    template<typename InputIterator>
    deque(InputIterator first, InputIterator last) {
        const size_type n = Distance(first, last);
        allocate_node_data(n);
        iterator cur = start;
        try {
            for(; cur != finish; ++cur, ++first) {
                construct(std::addressof(*cur), *first);
            }
        } catch(...) {
            destory(start, cur);
        }
    }

    deque(std::initializer_list<T> list): deque(list.begin(), list.end()) {}

    deque(const deque& other): deque(other.begin(), other.end()) {}

    deque(deque&& other): deque() {
        using std::swap;
        swap(start, other.start);
        swap(finish, other.finish);
        swap(map, other.map);
        swap(map_size, other.map_size);
    }

    ~deque() {
        destory_data(cbegin(), cend());
        destory_node(start.node, finish.node + 1);
        destory_map();
    }

    deque& operator=(const deque& other) {
        if(this == &other) return *this;
        assign(other.begin(), other.end());
        return *this;
    }

    deque& operator=(deque&& other) noexcept {
        if(this == &other) return *this;
        swap(other);
        other.clear();
        return *this;
    }

    deque& operator=(std::initializer_list<T> list) {
        assign(list.begin(), list.end());
        return *this;
    }

    void assign(size_type n, const T& value) {
        if(n <= size()) {
            erase_to_end(begin() + n);
            fill(begin(), end(), value);
        } else {
            fill(begin(), end(), value);
            fill_insert(end(), n - size(), value);
        }
    }

    template<typename InputIterator>
    void assign(InputIterator first, InputIterator last) {
        const size_type n = Distance(first, last);
        if(n <= size()) {
            erase_to_end(begin() + n);
            copy(first, last, begin());
        } else {
            InputIterator mid = first;
            Advance(mid, size());
            copy(first, mid, begin());
            range_insert(end(), mid, last);
        }
    }

    iterator begin() noexcept {
        return start;
    }

    const_iterator begin() const noexcept {
        return static_cast<const_iterator>(start);
    }

    const_iterator cbegin() const noexcept {
        return static_cast<const_iterator>(start);
    }

    iterator end() noexcept {
        return finish;
    }

    const_iterator end() const noexcept {
        return static_cast<const_iterator>(finish);
    }

    const_iterator cend() const noexcept {
        return static_cast<const_iterator>(finish);
    }

    size_type size() const noexcept {
        return finish - start;
    }

    bool empty() const noexcept {
        return start == finish;
    }

    reference front() noexcept {
        return *begin();
    }

    const_reference front() const noexcept {
        return *cbegin();
    }

    reference back() noexcept {
        iterator tmp = finish - 1;
        return *tmp;
    }

    const_reference back() const noexcept {
        iterator tmp = finish - 1;
        return *tmp;
    }

    reference operator[](size_type n) {
        return *(start + n);
    }

    template<typename... Args>
    void emplace_front(Args&&... args) {
        if(start.cur != start.first) {
            --start;
            construct(start.cur, std::forward<Args>(args)...);
        } else {
            resever_map_at_front();
            *(start.node - 1) = allo::allocate(deque_detail::deque_node_num(sizeof(T)));
            try {
                --start;
                construct(start.cur, std::forward<Args>(args)...);
            } catch(...) {
                ++start;
                allo::deallocate(*(start.node - 1), deque_detail::deque_node_num(sizeof(T)));
                throw;
            }
        }
    }

    template<typename U>
    void push_front(U&& value) {
        emplace_front(std::forward<U>(value));
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        if(finish.cur != finish.last - 1) {
            construct(finish.cur, std::forward<Args>(args)...);
            ++finish;
        } else {
            resever_map_at_back();
            *(finish.node + 1) = allo::allocate(deque_detail::deque_node_num(sizeof(T)));
            try {
                construct(finish.cur, std::forward<Args>(args)...);
            } catch(...) {
                allo::deallocate(*(finish.node + 1), deque_detail::deque_node_num(sizeof(T)));
            }
            ++finish;
        }
    }

    template<typename U>
    void push_back(U&& value) {
        emplace_back(std::forward<U>(value));
    }

    void pop_front() {
        destory(start.cur);
        ++start;
        if(start.cur == start.first) {
            allo::deallocate(*(start.node - 1), deque_detail::deque_node_num(sizeof(T)));
        }
    }

    void pop_back() {
        --finish;
        destory(finish.cur);
        if(finish.cur == finish.last - 1) {
            allo::deallocate(*(finish.node + 1), deque_detail::deque_node_num(sizeof(T)));
        }
    }

    template<typename... Args>
    iterator emplace(const_iterator position, Args&&... args) {
        iterator _position = position._const_cast();
        const size_type element_before = _position - begin();
        if(_position.cur == start.cur) {
            emplace_front(std::forward<Args>(args)...);
        } else if (_position.cur == finish.cur) {
            emplace_back(std::forward<Args>(args)...);
        } else {
            T copy = T(std::forward<Args>(args)...);
            if(element_before < size() / 2) {
                push_front(std::move(front()));
                //push_front可能造成map重新分配 _position会失效！
                _position = begin() + element_before + 1;
                move(begin() + 2, _position, begin() + 1);
                *(_position - 1) = std::move(copy);
            } else {
                push_back(std::move(back()));
                //push_back可能造成map重新分配 _position会失效！
                _position = begin() + element_before;
                backward_move(_position, end() - 2, end() - 1);
                *_position = std::move(copy);
            }
        }
        return begin() + element_before;
    }

    iterator insert(const_iterator position, T&& value) {
        return emplace(position, std::forward<T>(value));
    }

    iterator insert(const_iterator position, size_type n, const T& value) {
        return fill_insert(position._const_cast(), n, value);
    }

    template<typename InputIterator>
    iterator insert(const_iterator position, InputIterator first, InputIterator last) {
        return range_insert(position._const_cast(), first, last);
    }

    iterator insert(const_iterator position, std::initializer_list<T> list) {
        return range_insert(position._const_cast(), list.begin(), list.end());
    }

    iterator erase(const_iterator position) {
        iterator _position = position._const_cast();
        const size_type element_before = _position - begin();
        if(element_before < size() / 2) {
            if(_position != begin()) {
                backward_move(begin(), _position, _position + 1);
            }
            pop_front();
        } else {
            if(_position != end() - 1) {
                move(_position + 1, end(), _position);
            }
            pop_back();
        }
        return begin() + element_before;
    }

    iterator erase(const_iterator first, const_iterator last) {
        iterator _first = first._const_cast();
        iterator _last = last._const_cast();
        if(_first == _last) {
            return _first;
        }
        if(_first == begin() && _last == end()) {
            clear();
            return begin();
        } else {
            const size_type element_before = _first - begin();
            const size_type element_after = end() - _last;
            if(element_before < element_after) {
                if(_first != begin()) {
                    backward_move(begin(), _first, _last);
                }
                erase_to_begin(_last - element_before);
            } else {
                if(_last != end()) {
                    move(_last, end(), _first);
                }
                erase_to_end(_first + element_after);
            }
            return begin() + element_before;
        }
    }

    void swap(deque& other) noexcept {
        using std::swap;
        swap(start, other.start);
        swap(finish, other.finish);
        swap(map, other.map);
        swap(map_size, other.map_size);
    }

    void clear() {
        erase_to_end(cbegin());
    }

};

template<typename T, typename Allo>
inline void swap(deque<T, Allo>& lhs, deque<T, Allo>& rhs) noexcept {
	lhs.swap(rhs);
}

}

#endif