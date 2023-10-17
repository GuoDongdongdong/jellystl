#ifndef JELLY_ITERATOR
#define JELLY_ITERATOR

#include<cstddef> // std::ptrdiff_t

namespace jelly {

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template<class Iterator>
struct iterator_traits
{
	using iterator_category = typename Iterator::iterator_category;
	using value_type = typename Iterator::value_type;
	using distance_type = typename Iterator::distance_type;
	using pointer = typename Iterator::pointer;
	using reference = typename Iterator::reference;
};

template<typename T>
struct iterator_traits<T*>
{
	using iterator_category = random_access_iterator_tag;
	using value_type = T;
	using distance_type = std::ptrdiff_t;
	using pointer = T*;
	using reference = T&;
};

template<typename T>
struct iterator_traits<const T*>
{
	using iterator_category = random_access_iterator_tag;
	using value_type = T;
	using distance_type = std::ptrdiff_t;
	using pointer = const T*;
	using reference = const T&;
};

/*
	todo T* const属于什么类型迭代器？
*/
template<typename T>
struct iterator_traits<T* const> {

};

template<typename InputIterator>
inline typename iterator_traits<InputIterator>::distance_type _Distance(InputIterator first, InputIterator last, input_iterator_tag){
	typename iterator_traits<InputIterator>::distance_type distance{};
	for(; first != last; ++first){
		++distance;
	}
	return distance;
}

template<typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::distance_type _Distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag){
	return last - first;
}

template<typename Iterator>
inline typename iterator_traits<Iterator>::distance_type Distance(Iterator first, Iterator last){
	using iterator_category = typename iterator_traits<Iterator>::iterator_category;
	return _Distance(first, last, iterator_category());
}

template<typename Iterator, typename Distance>
inline void _Advane(Iterator& i, Distance n, input_iterator_tag){
	static_assert(n >= 0, "distance < 0!");
	while(n--){
		++i;
	}
}

template<typename Iterator, typename Distance>
inline void _Advane(Iterator& i, Distance n, bidirectional_iterator_tag){
	if (n > 0) {
		while (n--) {
			++i;
		}
	} else {
		while (n++) { 
			--i;
		}
	}
}

template<typename Iterator, typename Distance>
inline void _Advane(Iterator& i, Distance n, random_access_iterator_tag){
	i += n;
}

template<typename Iterator, typename Distance>
inline void Advance(Iterator& i, Distance n){
	using iterator_category = typename iterator_traits<Iterator>::iterator_category;
	_Advane(i, n, iterator_category());
}

}
#endif
