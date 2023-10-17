#ifndef JELLY_UNINITIALIZED
#define JELLY_UNINITIALIZED

#include "allocator.hpp"
#include "type_traits.hpp"
#include "algorithms.hpp"

namespace jelly{

template<class ForwardIterator, typename size_type, typename Value>
inline void uninitialized_fill_n(ForwardIterator first, size_type n, const Value& value) {
    using T = typename iterator_traits<ForwardIterator>::value_type;
    static_assert(std::is_constructible_v<const T&, Value>, "const T& can not construct from Value");
	if constexpr (std::is_trivial_v<T> && std::is_copy_assignable_v<T>) {
        if constexpr (is_byte_like_v<T>){
            ::memset(std::addressof(*first), static_cast<unsigned char>(value), n);
        } else if (std::is_scalar_v<T>){
            const Value tmp = value;
            for(; n > 0; --n, ++first){
                *first = tmp;
            }
        } else {
            for(; n > 0; --n, ++first){
                *first = value;
            }
        }
	}
	else {
		ForwardIterator cur = first;
        try{
            for(; n > 0; --n, ++cur){
                construct(std::addressof(*cur), value);
            }
        } catch (...){
            destory(first, cur);
            throw;
        }
	}
}

template<class ForwardIterator, typename size_type>
inline void uninitialized_default_n(ForwardIterator first, size_type n) {
	using T = typename iterator_traits<ForwardIterator>::value_type;
    static_assert(std::is_default_constructible_v<T>, "T can not default construct");
	if constexpr (std::is_trivial_v<T> && std::is_copy_assignable_v<T>){
        // todo 多付出了一个默认构造函数 可以在有优化为::memset(std::addressof(*first), 0, n * sizeof(T*)) 吗?
        uninitialized_fill_n(first, n, T());
	}
	else {
        ForwardIterator cur = first;
        try {
            for(; n > 0; --n, ++cur){
                construct(std::addressof(*cur));
            }
        } catch(...) {
            destory(first, cur);
            throw;
        }
	}
}

template<class InputIterator, class ForwardIterator>
inline void uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator target) {
	using T = typename iterator_traits<ForwardIterator>::value_type;
    using F = typename iterator_traits<InputIterator>::value_type;
    static_assert(std::is_constructible_v<T, F>, "T can not constrcut from F");

	if constexpr (std::is_trivial_v<T> && std::is_trivial_v<F> && std::is_assignable_v<T&, F&>) {
        copy(first, last, target);
	}
	else {
		ForwardIterator cur = target;
        try{
            for(; first != last; ++first, ++cur){
                construct(std::addressof(*cur), *first);
            }
        } catch (...){
            destory(target, cur);
            throw;
        }
	}
}

template<class InputIterator, class ForwardIterator>
inline void uninitialized_move(InputIterator first, InputIterator last, ForwardIterator target) {
	using T = typename iterator_traits<ForwardIterator>::value_type;
    using F = typename iterator_traits<InputIterator>::value_type;
    static_assert(std::is_constructible_v<T, F>, "T can not constrcut from F");

	if constexpr (std::is_trivial_v<T> && std::is_trivial_v<F> && std::is_assignable_v<T&, F&>) {
        move(first, last, target);
	}
	else {
		ForwardIterator cur = target;
        try{
            for(; first != last; ++first, ++cur){
                construct(std::addressof(*cur), std::move(*first));
            }
        } catch (...){
            destory(target, cur);
            throw;
        }
	}
}

}
#endif