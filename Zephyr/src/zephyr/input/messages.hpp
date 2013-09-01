/**
 * @file messages.hpp
 *
 * Message codes for messages related to input events.
 */

#ifndef ZEPHYR_INPUT_MESSAGES_HPP_
#define ZEPHYR_INPUT_MESSAGES_HPP_

#include <zephyr/core/Message.hpp>
#include <zephyr/util/static_hash.hpp>

namespace zephyr {
namespace input {
namespace msg {

constexpr core::address INPUT_SYSTEM = util::static_hash("input-system");

} /* namespace msg */
} /* namespace input */
} /* namespace zephyr */


#endif /* ZEPHYR_INPUT_MESSAGES_HPP_ */
