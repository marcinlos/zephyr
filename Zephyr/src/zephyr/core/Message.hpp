/**
 * @file Message.hpp
 */

#ifndef ZEPHYR_CORE_MESSAGE_HPP_
#define ZEPHYR_CORE_MESSAGE_HPP_

#include <boost/any.hpp>

namespace zephyr {
namespace core {


/**
 * Generic message structure. Uses boost::any to store arbitrary content.
 */
struct Message {
    std::uint32_t target;
    std::uint32_t type;
    boost::any data;
};

} /* namespace core */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_MESSAGE_HPP_ */
