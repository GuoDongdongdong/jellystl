#ifndef JELLY_VECTOR
#define JELLY_VECTOR

#include<initializer_list>
#include"allocator.hpp"
#include"algorithms.hpp"
#include"uninitialized.hpp"

namespace jelly {

template<class T, class allo = allocator<T> >
class vector {
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using size_type = std::size_t;
	using distance_type = std::ptrdiff_t;
	using iterator = T*;
	using const_iterator = const T*;

private:
	iterator start;
	iterator finish;
	iterator end_of_storge;

	template<typename InputIterator>
	void assign(InputIterator first, InputIterator last){
		typename iterator_traits<InputIterator>::distance_type assign_size = Distance(first, last);
		if (capacity() < assign_size) { // 需要扩容
			pointer new_start = allo::allocate(assign_size);
			try {
				uninitialized_copy(first, last, new_start);
			} catch (...) {
				allo::deallocate(new_start, assign_size);
				throw;
			}
			clear();
			allo::deallocate(start, capacity());
			start = new_start;
			finish = end_of_storge = begin() + assign_size;
		} else if (size() < assign_size) {
			InputIterator mid = first;
			Advance(mid, size());
			copy(first, mid, begin());
			uninitialized_copy(mid, last, finish);
			finish = begin() + assign_size;
		} else {
			copy(first, last, begin());
			erase_to_end(begin() + assign_size);
		}
	}

	void erase_to_end(const_iterator position){
		destory(position, cend());
		finish = begin() + (position - cbegin());
	}

public:
	vector():start(), finish(), end_of_storge(){}

	explicit vector(size_type n) {
		start = allo::allocate(n);
		// 注意finish要先初始化为start 否则异常抛出会desotry两次！
		finish = start;
		end_of_storge = start + n;
		uninitialized_default_n(start, n);
		finish = start + n;
	}
	
	vector(size_type n, const T& value) {
		start = allo::allocate(n);
		finish = start;
		end_of_storge = start + n;
		uninitialized_fill_n(start, n, value);
		finish = start + n;
	}

	template<typename InputIterator>
	vector(InputIterator first, InputIterator last) {
		size_type n = last - first;
		start = allo::allocate(n);
		finish = start;
		end_of_storge = start + n;
		uninitialized_copy(first, last, start);
		finish = start + n;
	}

	vector(const vector& other) {
		size_type n = other.size();
		start = allo::allocate(n);
		finish = start;
		end_of_storge = start + n;
		uninitialized_copy(other.cbegin(), other.cend(), start);
		finish = start + n;
	}

	vector(vector&& other):start(other.start), finish(other.finish), end_of_storge(other.end_of_storge) {
		other.start = other.finish = other.end_of_storge = nullptr;
	}

	vector(std::initializer_list<T> list): vector(list.begin(), list.end()) {}

	vector& operator=(const vector& other) {
		if (this == &other) return *this;
		assign(other.cbegin(), other.cend());
		return *this;
	}

	vector& operator=(vector&& other) noexcept {
		if (this == &other) return *this;
		clear();
		allo::deallocate(start, capacity());
		start = other.start;
		finish = other.finish;
		end_of_storge = other.end_of_storge;

		other.start = nullptr;
		other.finish = nullptr;
		other.end_of_storge = nullptr;
		return *this;
	}

	vector& operator=(std::initializer_list<T> list){
		assign(list.begin(), list.end());
		return *this;
	}

	~vector() {
		clear();
		allo::deallocate(start, capacity());
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

	size_type size() const {
		return cend() - cbegin();
	}

	size_type capacity() const {
		return end_of_storge - cbegin();
	}

	bool empty() const {
		return cbegin() == cend();
	}

	reference operator[](distance_type n) {
		return *(begin() + n);
	}

	reference front() {
		return *begin();
	}

	const_reference front() const {
		return *cbegin();
	}

	reference back() {
		return *(end() - 1);
	}
	
	const_reference back() const {
		return *(cend() - 1);
	}
	
	void reserve(size_type new_capacity){
		if (new_capacity > capacity()) {
			const size_type old_size = size();
			pointer new_start = allo::allocate(new_capacity);
			try {
				move(begin(), end(), new_start);
			} catch(...) {
				allo::deallocate(new_start, new_capacity);
				throw;
			}
			clear();
			allo::deallocate(start, capacity());
			start = new_start;
			finish = begin() + old_size;
			end_of_storge = begin() + new_capacity;
		}
	}

	template<typename U>
	void push_back(U&& value){
		if (end() != end_of_storge) {
			construct(end(), std::forward<U>(value));
			++finish;
		} else {
			const size_type old_size = size();
			const size_type new_size = old_size ? 2 * old_size : 1;
			pointer new_start = allo::allocate(new_size);
			try {
				construct(new_start + old_size, std::forward<U>(value));
				uninitialized_move(begin(), end(), new_start);
			} catch (...) {
				allo::deallocate(new_start, new_size);
				throw;
			}
			clear();
			allo::deallocate(start, capacity());
			start = new_start;
			finish = start + old_size + 1;
			end_of_storge = start + new_size;
		}
	}
	
	template<typename... Args>
	void emplace_back(Args&&... args) {
		if (end() != end_of_storge) {
			construct(end(), std::forward<Args>(args)...);
			++finish;
		} else {
			const size_type old_size = size();
			const size_type new_size = old_size ? 2 * old_size : 1;
			pointer new_start = allo::allocate(new_size);
			try {
				construct(new_start + old_size, std::forward<Args>(args)...);
				uninitialized_move(begin(), end(), new_start);
			} catch (...) {
				allo::deallocate(new_start, new_size);
				throw;
			}
			clear();
			allo::deallocate(start, capacity());
			start = new_start;
			finish = start + old_size + 1;
			end_of_storge = start + new_size;
		}
	}

	void pop_back() {
		--finish;
		destory(finish);
	}

	iterator erase(const_iterator position) {
		iterator _position = begin() + (position - cbegin());
		if (_position + 1 != end()) {
			move(_position + 1, end(), _position);
		}
		--finish;
		destory(std::addressof(*finish));
		return _position;
	}

	iterator erase(const_iterator first, const_iterator last) {
		iterator _first = begin() + (first - cbegin());
		iterator _last = begin() + (last - cbegin());

		if (_first != _last) {
			if (_last != end()) {
				move(_last, end(), _first);
			}
			erase_to_end(_first + (end() - _last));
		}
		return _first;
	}

	iterator insert(const_iterator position, const T& value, size_type n) {
		iterator _position = begin() + (position - cbegin());
		const size_type offset = _position - begin();
		if (n + size() <= capacity()) {
			if(_position == end()) {
				uninitialized_fill_n(end(), n, value);
			} else {
				const size_type position_after_size = end() - _position;
				if (n <= position_after_size) {
					uninitialized_move(end() - n, end(), end());
					backward_move(_position, end() - n, end());
					fill(_position, _position + n, value);
				} else {
					uninitialized_move(_position, end(), end() + n - position_after_size);
					uninitialized_fill_n(end(), n - position_after_size, value);
					fill(_position, end(), value);
				}
			}
			finish += n;
 		} else {
			const size_type old_size = size();
			const size_type new_size = old_size + std::max(old_size, n);
			pointer new_start = allo::allocate(new_size);
			pointer new_finish = new_start;

			const size_type element_before_size = _position - begin();
			const size_type element_after_size = end() - _position;
			try {
				uninitialized_move(begin(), _position, new_start);
				new_finish += element_before_size;
				uninitialized_fill_n(new_finish, n, value);
				new_finish += n;
				uninitialized_move(_position, end(), new_finish);
				new_finish += element_after_size;
			} catch(...) {
				destory(new_start, new_finish);
				allo::deallocate(new_start, new_size);
				throw;
			}
			clear();
			allo::deallocate(begin(), capacity());
			start = new_start;
			finish = new_finish;
			end_of_storge = begin() + new_size;
		}
		return begin() + offset;
	}

	template<typename U>
	iterator insert(const_iterator position, U&& value) {
		iterator _position = begin() + (position - cbegin());
		const size_type offset = _position - begin();
		if (finish != end_of_storge) {
			if (_position == end()) {
				construct(std::addressof(*finish), std::forward<U>(value));
				++finish;
			} else {
				construct(std::addressof(*end()), std::move(back()));
				++finish;
				backward_move(_position, end() - 2, end() - 1);
				*_position = std::forward<U>(value);
			}
		} else {
			const size_type old_size = size();
			const size_type new_size = old_size ? 2 * old_size : 1;
			pointer new_start = allo::allocate(new_size);
			pointer new_finish = new_start;
			try {
				uninitialized_move(begin(), _position, new_start);
				new_finish += (_position - begin());
				construct(new_finish, std::forward<U>(value));
				++new_finish;
				uninitialized_move(_position, end(), new_finish);
				new_finish += end() - _position;
			} catch (...) {
				destory(new_start, new_finish);
				allo::deallocate(new_start, new_size);
				throw;
			}
			clear();
			allo::deallocate(begin(), capacity());
			start = new_start;
			finish = start + old_size + 1;
			end_of_storge = start + new_size;
		}
		return begin() + offset;
	}
	
	template<typename InputIterator>
	iterator insert(const_iterator position, InputIterator first, InputIterator last){
		const size_type n = Distance(first, last);
		iterator _position = begin() + (position - cbegin());
		const size_type offset = _position - begin();
		if (size() + n <= capacity()) {
			if (_position == end()){
				uninitialized_copy(first, last, end());
			} else {
				const size_type element_after_size = end() - _position;
				if (n <= element_after_size) {
					uninitialized_move(end() - n, end(), end());
					backward_move(_position, end() - n, end());
					copy(first, last, _position);
				} else {
					InputIterator mid = first;
					Advance(mid, element_after_size);
					uninitialized_move(_position, end(), end() + (n - element_after_size));
					uninitialized_copy(mid, last, end());
					copy(first, mid, _position);
				}
			}
			finish += n;
		} else {
			const size_type old_size = size();
			const size_type new_size = old_size + std::max(old_size, n);
			const size_type element_before_size = _position - begin();
			const size_type element_after_size = end() - _position;
			iterator new_start = allo::allocate(new_size);
			iterator new_finish = new_start;
			try {
				uninitialized_move(begin(), _position, new_start);
				new_finish += element_before_size;
				uninitialized_copy(first, last, new_finish);
				new_finish += n;
				uninitialized_move(_position, end(), new_finish);
				new_finish += element_after_size;
			} catch(...) {
				destory(new_start, new_finish);
				allo::deallocate(new_start, new_size);
			}
			clear();
			allo::deallocate(start, capacity());
			start = new_start;
			finish = new_finish;
			end_of_storge = begin() + new_size;
		}
		return begin() + offset;
	}

	iterator insert(const_iterator position, std::initializer_list<T> list) {
		return insert(position, list.begin(), list.end());
	}

	template<typename... Args>
	iterator emplace(const_iterator position, Args&&... args) {
		iterator _position = begin() + (position - cbegin());
		const size_type offset = _position - begin();
		if (finish != end_of_storge) {
			if (_position == end()) {
				construct(std::addressof(*finish), std::forward<Args>(args)...);
				++finish;
			} else {
				construct(std::addressof(*end()), std::move(back()));
				++finish;
				backward_move(_position, end() - 2, end() - 1);
				T copy(std::forward<Args>(args)...);
				*_position = std::move(copy);
			}
		} else {
			const size_type old_size = size();
			const size_type new_size = old_size ? 2 * old_size : 1;
			pointer new_start = allo::allocate(new_size);
			pointer new_finish = new_start;
			try {
				uninitialized_move(begin(), _position, new_start);
				new_finish += (_position - begin());
				construct(new_finish, std::forward<Args>(args)...);
				++new_finish;
				uninitialized_move(_position, end(), new_finish);
				new_finish += end() - _position;
			} catch (...) {
				destory(new_start, new_finish);
				allo::deallocate(new_start, new_size);
				throw;
			}
			clear();
			allo::deallocate(begin(), capacity());
			start = new_start;
			finish = start + old_size + 1;
			end_of_storge = start + new_size;
		}
		return begin() + offset;
	}

	void clear() {
		erase(begin(), end());
	}

	void swap(vector& other) noexcept {
		using std::swap;
		swap(start, other.start);
		swap(finish, other.finish);
		swap(end_of_storge, other.end_of_storge);
	}

};

template<typename T, typename Allo>
inline void swap(vector<T, Allo>& lhs, vector<T, Allo>& rhs) noexcept {
	lhs.swap(rhs);
}

}
#endif // !JELLY_VECTOR