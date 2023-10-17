#ifndef JELLY_STACK
#define JELLY_STACK

#include"deque.hpp"

namespace jelly {

template<typename T, typename Sequence = deque<T> >
class stack {
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
    using self = stack;

public:

    stack() = default;

    stack(const self&) = default;

    stack(self&&) = default;

    ~stack() = default;

    self& operator=(const self&) = default;

    self& operator=(self&&) = default;

    size_type size() const noexcept {
        return sequence.size();
    }

    bool empty() const noexcept {
        return sequence.empty();
    }

    reference top() noexcept {
        return sequence.back();
    }
    
    const_reference top() const noexcept {
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
        sequence.pop_back();
    }

    void swap(self& other) {
        using std::swap;
        swap(sequence, other.sequence);
    }

};

template<typename T, typename Sequence>
inline void swap(stack<T,Sequence>& lhs, stack<T,Sequence>& rhs) {
    lhs.swap(rhs);
}

}

#endif