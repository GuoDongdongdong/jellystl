#ifndef JELLY_ALGORITHMS
#define JELLY_ALGORITHMS

#include"type_traits.hpp"
#include"functional.hpp"
#include"iterator.hpp"

namespace jelly {

template<bool Move, bool Simple, class iterator_category>
struct __copy_move{
    template<typename InputIterator, typename ForwordIterator>
    static void copy_move(InputIterator first, InputIterator last, ForwordIterator target){
        for(; first != last; ++first, ++target){
            *target = *first;
        }
    }
};

template<bool Move>
struct __copy_move<Move, true, random_access_iterator_tag>{
    template<typename T>
    static void copy_move(const T* first, const T* last, T* target){
		static_assert(std::conditional_t<Move, std::is_move_assignable<T>, std::is_copy_assignable<T> >::value, "T can not assignable");
		auto distance = last - first;
		if(distance){
			::memmove(target, first, distance * sizeof(T));
		}
    }
};

template<>
struct __copy_move<false, false, random_access_iterator_tag>{
    template<typename InputIterator, typename ForwardIterator>
    static void copy_move(InputIterator first, InputIterator last, ForwardIterator target){
        typename iterator_traits<InputIterator>::distance_type distance = last - first;
        for(; distance > 0; --distance, ++target, ++first){
            *target = *first;
        }
    }
};

template<>
struct __copy_move<true, false, random_access_iterator_tag>{
    template<typename InputIterator, typename ForwardIterator>
    static void copy_move(InputIterator first, InputIterator last, ForwardIterator target){
        typename iterator_traits<InputIterator>::distance_type distance = last - first;
        for(; distance > 0; --distance, ++target, ++first){
            *target = std::move(*first);
        }
    }
};

template<typename iterator_category>
struct __copy_move<true, false, iterator_category>{
    template<typename InputIterator, typename ForwardIterator>
    static void copy_move(InputIterator first, InputIterator last, ForwardIterator target){
        for(; first != last; ++first, ++target){
            *target = std::move(*first);
        }
    }
};

template<bool Move, typename InputIterator, typename OutputIterator>
inline void copy_move(InputIterator first, InputIterator last, OutputIterator target){
	using T = typename iterator_traits<OutputIterator>::value_type;
    using F = typename iterator_traits<InputIterator>::value_type;
	using iterator_category = typename iterator_traits<InputIterator>::iterator_category;
	constexpr bool simple = std::is_trivially_copyable_v<T> && std::is_same_v<T, F> && std::is_pointer_v<InputIterator> && std::is_pointer_v<OutputIterator>;
    __copy_move<Move, simple, iterator_category>::copy_move(first, last, target);
}

template<typename InputIterator, typename OutputIterator>
inline void copy(InputIterator first, InputIterator last, OutputIterator target){
	copy_move<false>(first, last, target);
}

template<typename InputIterator, typename OutputIterator>
inline void move(InputIterator first, InputIterator last, OutputIterator target){
	copy_move<true>(first, last, target);
}

template<bool Move, bool Simple, class iterator_category>
struct __backward_copy_move{
    template<typename InputIterator, typename ForwordIterator>
    static void backward_copy_move(InputIterator first, InputIterator last, ForwordIterator target){
        while(first != last){
            *(--target) = *(--last);
        }
    }
};

template<bool Move>
struct __backward_copy_move<Move, true, random_access_iterator_tag>{
    template<typename T>
    static void backward_copy_move(const T* first, const T* last, T* target){
		static_assert(std::conditional_t<Move, std::is_move_assignable<T>, std::is_copy_assignable<T> >::value, "T can not assignable");
		auto distance = last - first;
		if(distance){
			::memmove(target - distance, first, distance * sizeof(T));
		}
    }
};

template<>
struct __backward_copy_move<false, false, random_access_iterator_tag>{
    template<typename InputIterator, typename ForwardIterator>
    static void backward_copy_move(InputIterator first, InputIterator last, ForwardIterator target){
        typename iterator_traits<InputIterator>::distance_type distance = last - first;
        for(; distance > 0; --distance){
            *(--target) = *(--last);
        }
    }
};

template<>
struct __backward_copy_move<true, false, random_access_iterator_tag>{
    template<typename InputIterator, typename ForwardIterator>
    static void backward_copy_move(InputIterator first, InputIterator last, ForwardIterator target){
        typename iterator_traits<InputIterator>::distance_type distance = last - first;
        for(; distance > 0; --distance){
            *(--target) = std::move(*(--last));
        }
    }
};

template<typename iterator_category>
struct __backward_copy_move<true, false, iterator_category>{
    template<typename InputIterator, typename ForwardIterator>
    static void backward_copy_move(InputIterator first, InputIterator last, ForwardIterator target){
        while(first != last){
            *(--target) = std::move(*(--last));
        }
    }
};


template<bool Move, typename BidirectionalIterator_1, typename BidirectionalIterator_2>
inline void backward_copy_move(BidirectionalIterator_1 first, BidirectionalIterator_1 last, BidirectionalIterator_2 target){
    using T = typename iterator_traits<BidirectionalIterator_1>::value_type;
    using F = typename iterator_traits<BidirectionalIterator_2>::value_type;
	using iterator_category = typename iterator_traits<BidirectionalIterator_1>::iterator_category;
	constexpr bool simple = std::is_trivially_copyable_v<T> && std::is_same_v<T, F> && std::is_pointer_v<BidirectionalIterator_1> && std::is_pointer_v<BidirectionalIterator_2>;
    __backward_copy_move<Move, simple, iterator_category>::backward_copy_move(first, last, target);
}

template<typename BidirectionalIterator_1, typename BidirectionalIterator_2>
inline void backward_copy(BidirectionalIterator_1 first, BidirectionalIterator_1 last, BidirectionalIterator_2 target){
    backward_copy_move<false>(first, last, target);
}

template<typename BidirectionalIterator_1, typename BidirectionalIterator_2>
inline void backward_move(BidirectionalIterator_1 first, BidirectionalIterator_1 last, BidirectionalIterator_2 target){
    backward_copy_move<true>(first, last, target);
}

template<typename InputIterator, typename T>
inline void fill(InputIterator first, InputIterator last, const T& value){
    if constexpr (std::is_scalar_v<T>){
        if constexpr (is_byte_like_v<T>){
            ::memset(std::addressof(*first), static_cast<unsigned char>(value), last - first);
        }
        else{
            const T tmp = value;
            for(; first != last; ++first){
                *first = tmp;
            }
        }
    }
    else{
        for(; first != last; ++first){
            *first = value;
        }
    }
}

//[first, last)是堆，将顶部元素放置最后一个元素处，重新调整使得[first, last - 1)为一个堆。
template<typename RandomAccessIterator, typename Compare = less<typename iterator_traits<RandomAccessIterator>::value_type>>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare compare = Compare()) {
    using Distance = typename iterator_traits<RandomAccessIterator>::distance_type;
    Distance len = last - first;
    using std::swap;
    swap(*first, *(last - 1));
    adjust_heap(first, len - 1, Distance(0), compare);
}

// 假设[first, last - 1)是堆，last - 1是刚被push的元素位置，重新调整使得[first, last)为一个堆。
template<typename RandomAccessIterator, typename Compare = less<typename iterator_traits<RandomAccessIterator>::value_type>>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare compare = Compare()) {
    using Distance = typename iterator_traits<RandomAccessIterator>::distance_type;
    Distance len = last - first;
    Distance child = len - 1;
    Distance parent = (child - 1) / 2;
    using std::swap;
    while(child > 0) {
        if(compare(*(first + parent), *(first + child))) {
            swap(*(first + parent), *(first + child));
        }
        child = parent;
        parent = (child - 1) / 2;
    }
}

// 以index为子树调整为堆，并假设左右子树已经是堆。
template<typename RandomAccessIterator, typename Distance, typename Compare>
inline void adjust_heap(RandomAccessIterator first, Distance len, Distance index, Compare compare) {
    Distance parent = index;
    Distance child = index;
    using std::swap;
    while(parent < (len - 1) / 2) { // 第一个叶节点是 len / 2，但是当len为偶数时，最后一个非叶节点只有左孩子，此时另外处理，因此不是小于len / 2，要规避掉len为偶数的情况。
        child = 2 * parent + 2;
        if(compare(*(first + parent), *(first + child))) {
            if(compare(*(first + child), *(first + child - 1))) {
                --child;
            }
            swap(*(first + parent), *(first + child));
            parent = child;
        } else {
            if(compare(*(first + parent), *(first + child - 1))) {
                swap(*(first + parent), *(first + child - 1));
                parent = child - 1;
            } else {
                return ;
            }
        }
    }
    if((len & 1) == 0 && parent == (len - 1) / 2) { // len为偶数时最后一个非叶节点只有左孩子。
        child = 2*parent + 1;
        if(compare(*(first + parent), *(first + child))) {
            swap(*(first + parent), *(first + child));
        }
    }
}

// 堆是一个完全二叉树，假设节点从 0 开始，最后一个非叶节点为 n / 2 - 1。
// 对于节点x，父节点为 (x - 1) / 2，左孩子为 2x + 1，右孩子为 2x + 2。
template<typename RandomAccessIterator, typename Compare = less<typename iterator_traits<RandomAccessIterator>::value_type>>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare compare = Compare()) {
    using Distance = typename iterator_traits<RandomAccessIterator>::distance_type;
    Distance len = last - first;
    if(len < 2) return ;
    Distance non_leaf_index = len / 2 - 1;
    while(non_leaf_index >= 0) {
        adjust_heap(first, len, non_leaf_index, compare);
        --non_leaf_index;
    }
}

}

#endif
