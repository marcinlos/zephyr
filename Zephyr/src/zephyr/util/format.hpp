/**
 * @file
 *
 * @brief String formatting function
 */
#ifndef ZEPHYR_UTIL_FORMAT_HPP_
#define ZEPHYR_UTIL_FORMAT_HPP_

#include <sstream>
#include <string>
#include <utility>

namespace zephyr {
namespace util {

/** String used to denote places where arguments are to be inserted */
const std::string PLACEHOLDER = "{}";

namespace {

/** Auxilary closure */
template<typename T>
inline void useNext(std::ostringstream& s, const std::string& format,
        std::string::size_type& pos, const T& val) {
    auto next = format.find(PLACEHOLDER, pos);
    if (next == std::string::npos) {
        next = format.size();
    }
    s.write(format.data() + pos, next - pos);
    pos = next + (next == format.size() ? 0 : PLACEHOLDER.size());
    s << val;
}

}

/**
 * @brief String formatting function
 *
 * Function providing minimalistic Python-style type-safe string formatting.
 * Arguments are inserted at occurences of @ref PLACEHOLDER (@c {} by default)
 * and are formatted using stream operator (@c <<)
 */
template<typename... Args>
inline std::string format(const std::string& format, const Args&... args) {
    std::ostringstream s;
    std::string::size_type pos = 0;

#pragma GCC diagnostic pop
#pragma GCC diagnostic ignored "-Wunused-variable"

    // Tricky way to iterate over the arguments
    auto exp { (useNext(s, format, pos, args), true)... };

#pragma GCC diagnostic push

    // Append remaining part of a string
    if (!format.empty()) {
        s.write(format.data() + pos, format.size() - pos);
    }
    return s.str();
}

} /* namespace util */
} /* namespace zephyr */

#endif /* ZEPHYR_UTIL_FORMAT_HPP_ */
