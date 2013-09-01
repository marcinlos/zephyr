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

//using util::operator"" _h;


const core::address INPUT_SYSTEM = util::static_hash("input-system");


const std::uint32_t KEYBOARD_EVENT = util::static_hash("keyboard-event");

const std::uint32_t BUTTON_EVENT = util::static_hash("button-event");

const std::uint32_t CURSOR_EVENT = util::static_hash("cursor-move-event");

const std::uint32_t SCROLL_EVENT = util::static_hash("mouse-scroll-event");

const std::uint32_t FOCUS_EVENT = util::static_hash("window-focus-event");


} /* namespace msg */
} /* namespace input */
} /* namespace zephyr */


#endif /* ZEPHYR_INPUT_MESSAGES_HPP_ */
