/**
 * @file messages.hpp
 *
 * Message codes for global system events.
 */

#ifndef ZEPHYR_WINDOW_MESSAGES_HPP_
#define ZEPHYR_WINDOW_MESSAGES_HPP_

#include <zephyr/util/static_hash.hpp>


namespace zephyr {
namespace window {
namespace msg {

const core::address WINDOW = util::static_hash("window-system");

const std::uint32_t FULLSCREEN_ON = util::static_hash("fullscreen-on");
const std::uint32_t FULLSCREEN_OFF = util::static_hash("fullscreen-off");
const std::uint32_t TOGGLE_FULLSCREEN = util::static_hash("toggle-fullscreen");

} /* namespace msg */
} /* namespace window */
} /* namespace zephyr */


#endif /* ZEPHYR_WINDOW_MESSAGES_HPP_ */
