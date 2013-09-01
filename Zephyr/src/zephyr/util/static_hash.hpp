/**
 * @file static_hash.hpp
 */

#ifndef ZEPHYR_UTIL_STATIC_HASH_HPP_
#define ZEPHYR_UTIL_STATIC_HASH_HPP_

#include <cstdint>

namespace zephyr {
namespace util {

namespace impl {

constexpr std::size_t step(const char* text, std::size_t hash) {
    return *text + (hash << 6) + (hash << 16) - hash;
}

constexpr std::size_t partial_hash(const char* text, std::size_t hash) {
    return *text ?
        partial_hash(text + 1, step(text, hash))
        : hash;
}

} /* namespace impl */

constexpr std::size_t static_hash(const char* text) {
    return impl::partial_hash(text, 0);
}


inline constexpr std::uint32_t operator"" _h(const char* str, std::size_t) {
    return zephyr::util::static_hash(str);
}

} /* namespace util */
} /* namespace zephyr */


#endif /* ZEPHYR_UTIL_STATIC_HASH_HPP_ */
