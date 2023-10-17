#ifndef JELLY_TYPE_TRAITS
#define JELLY_TYPE_TRAITS

#include<type_traits>

namespace jelly{

template<typename T>
struct is_byte_like: public std::integral_constant<bool, false>{};

template<>
struct is_byte_like<char>: public std::integral_constant<bool, true>{};

template<>
struct is_byte_like<signed char>: public std::integral_constant<bool, true>{};

template<>
struct is_byte_like<unsigned char>: public std::integral_constant<bool, true>{};

template<>
struct is_byte_like<wchar_t>: public std::integral_constant<bool, true>{};

template<typename T>
inline constexpr bool is_byte_like_v = is_byte_like<T>::value;

}

#endif