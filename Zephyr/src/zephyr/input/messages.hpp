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


const core::address INPUT_SYSTEM = util::static_hash("input-system");

/**
 * @defgroup Input system messages
 *
 * Messages sent to the input system, carrying information about input events.
 */
/// @{

const std::uint32_t KEYBOARD_EVENT = util::static_hash("keyboard-event");

const std::uint32_t BUTTON_EVENT = util::static_hash("button-event");

const std::uint32_t CURSOR_EVENT = util::static_hash("cursor-move-event");

const std::uint32_t SCROLL_EVENT = util::static_hash("mouse-scroll-event");

const std::uint32_t FOCUS_EVENT = util::static_hash("window-focus-event");

/// @}

const core::address INPUT_SOURCE = util::static_hash("input-source");

/**
 * @defgroup Input source messages
 *
 * Messages sent to the input source, controlling its behaviour and settings.
 */
/// @{

/**
 * Informs the input source that relative mouse movement is used instead of
 * absolute cursor position, and so the mouse should not be confined to the
 * window area nor displayed.
 */
const std::uint32_t MOUSE_REL = util::static_hash("mouse-free");

/**
 * Informs the input source that absolute cursor position is used, and so the
 * mouse should be confined to the window area, andcursor should be displayed.
 */
const std::uint32_t MOUSE_ABS = util::static_hash("mouse-window");

/// @}


} /* namespace msg */
} /* namespace input */
} /* namespace zephyr */


#endif /* ZEPHYR_INPUT_MESSAGES_HPP_ */
