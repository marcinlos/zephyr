/**
 * @file make_unique.hpp
 *
 * Provides a function ommited from the standard by oversight.
 */

#ifndef ZEPHYR_UTIL_MAKE_UNIQUE_HPP_
#define ZEPHYR_UTIL_MAKE_UNIQUE_HPP_

#include <memory>

namespace zephyr {
namespace util {

/**
 * Creates @ref unique_ptr<> forwarding specified arguments to appropriate
 * constructor.
 */
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

} /* namespace util */
} /* namespace zephyr */

#endif /* ZEPHYR_UTIL_MAKE_UNIQUE_HPP_ */
