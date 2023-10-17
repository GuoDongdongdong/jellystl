#ifndef JELLY_FUNCTIONAL
#define JELLY_FUNCTIONAL

namespace jelly {

template<typename T>
struct less {
    constexpr bool operator()(const T& lhs, const T& rhs) {
        return lhs < rhs;
    }
};

}

#endif