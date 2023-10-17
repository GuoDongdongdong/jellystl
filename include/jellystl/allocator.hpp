#ifndef JELLY_ALLOCATOR
#define JELLY_ALLOCATOR

#include<cstring> // memmove
#include<new> // placement new

#include"iterator.hpp"

namespace jelly {

template<typename T1, typename... Args>
inline void construct(T1* ptr, Args&&... args) {
	new (ptr) T1(std::forward<Args>(args)...);
}

template<typename T>
inline void destory(T* ptr) {
	ptr->~T();
}

template<typename ForwardIterator>
inline void destory(ForwardIterator first, ForwardIterator last) {
	using T = typename iterator_traits<ForwardIterator>::value_type;
	if constexpr (std::is_trivially_destructible_v<T>) {
		return;
	}
	for (; first != last; ++first) {
		destory(std::addressof(*first));
	}
}

template<typename T>
class allocator {
public:
	template<typename U>
	struct rebind {
		using other = allocator<U>;
	};
	
	static T* allocate(size_t n) {
		return n == 0 ? nullptr : static_cast<T*>(::operator new(n * sizeof(T)));
	}

	static T* allocate() {
		return static_cast<T*>(::operator new(sizeof(T)));
	}

	static void deallocate(T* ptr, size_t n) noexcept {
		::operator delete(ptr, n * sizeof(T));
	}

	static void deallocate(T* ptr) noexcept {
		::operator delete(ptr, sizeof(T));
	}
};

}
#endif