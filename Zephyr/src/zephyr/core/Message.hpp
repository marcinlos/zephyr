/**
 * @file Message.hpp
 */

#ifndef ZEPHYR_CORE_MESSAGE_HPP_
#define ZEPHYR_CORE_MESSAGE_HPP_

#include <zephyr/util/format.hpp>
#include <zephyr/util/Any.hpp>
#include <iostream>

namespace zephyr {
namespace core {

/** Type of the message target */
typedef std::uint32_t address;

/**
 * Generic message structure. Uses boost::any to store arbitrary content.
 */
struct Message {
    address target;
    std::uint32_t type;
    util::Any data;
};

inline std::ostream& operator << (std::ostream& os, const Message& message) {
    return os << util::format("Message{to={}, type={}, data={}}",
            message.target, message.type, message.data);
}

/**
 * Address ment for sending/receiving important system-wide messages, like
 * quitting.
 */
constexpr address BROADCAST = -1;


} /* namespace core */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_MESSAGE_HPP_ */
