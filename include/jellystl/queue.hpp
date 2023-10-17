#ifndef JELLY_QUEUE
#define JELLY_QUEUE

#include<initializer_list>
#include<bits/move.h>

#include"deque.hpp"
#include"vector.hpp"
#include"functional.hpp"

namespace jelly {

template<typename T, typename Sequence = deque<T> >
class queue {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = typename Sequence::size_type;
    using distance_type = typename Sequence::distance_type;

private:
    Sequence sequence;
    using self = queue;

public:
    queue() = default;

    queue(const self&) = default;

    queue(self&&) = default;

    ~queue() = default;

    self& operator=(const self&) = default;

    self& operator=(self&&) = default;

    size_type size() const noexcept {
        return sequence.size();
    }

    bool empty() const noexcept {
        return sequence.empty();
    }

    reference front() noexcept {
        return sequence.front();
    }
    
    const_reference front() const noexcept {
        return sequence.front();
    }

    reference back() noexcept {
        return sequence.back();
    }
    
    const_reference back() const noexcept {
        return sequence.back();
    }

    template<typename U>
    void push(U&& value) {
        sequence.push_back(std::forward<U>(value));
    }
    
    template<typename... Args>
    void emplace(Args&&... args) {
        sequence.emplace_back(std::forward<Args>(args)...);
    }

    void pop() {
        sequence.pop_front();
    }

    void swap(self& other) {
        using std::swap;
        swap(sequence, other.sequence);
    }

};

template<typename T, typename Sequence>
inline void swap(queue<T,Sequence>& lhs, queue<T,Sequence>& rhs) {
    lhs.swap(rhs);
}

template<typename T, typename Sequence = vector<T>, typename Compre = less<T> >
class priority_queue {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = typename Sequence::size_type;
    using distance_type = typename Sequence::distance_type;

private:
    Sequence sequence;
    Compre compare;
    using self = priority_queue;

public:
    priority_queue() = default;

    template<typename InputIterator>
    priority_queue(InputIterator first, InputIterator last): sequence(), compare() {
        make_heap(first, last, compare);
    }
    priority_queue(std::initializer_list<T> list): priority_queue(list.begin(), list.end()) {}

    priority_queue(const self&) = default;

    priority_queue(self&&) = default;

    ~priority_queue() = default;

    self& operator=(const self&) = default;

    self& operator=(self&&) = default;

    size_type size() const noexcept {
        return sequence.size();
    }

    bool empty() const noexcept {
        return sequence.empty();
    }

    const_reference front() const {
        return sequence.front();
    }

    template<typename U>
    void push(U&& value) {
        sequence.push_back(std::forward<U>(value));
        push_heap(sequence.begin(), sequence.end());
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        sequence.emplace_back(std::forward<Args>(args)...);
        push_heap(sequence.begin(), sequence.end());
    }

    void pop() {
        pop_heap(sequence.begin(), sequence.end());
        sequence.pop_back();
    }

    void swap(self& other) {
        using std::swap;
        swap(other.sequence, other.sequence);
        swap(other.compare, other.compare);
    }

};

template<typename T, typename Sequence>
inline void swap(priority_queue<T,Sequence>& lhs, priority_queue<T,Sequence>& rhs) {
    lhs.swap(rhs);
}

}

#endif