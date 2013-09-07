/**
 * @file messages.hpp
 *
 * Message codes for global system events.
 */

#ifndef ZEPHYR_MESSAGES_HPP_
#define ZEPHYR_MESSAGES_HPP_

#include <zephyr/util/static_hash.hpp>


namespace zephyr {
namespace msg {

//using util::operator"" _h;

/**
 * @defgroup SystemTasks System tasks
 *
 * Predefined tasks, realizing necessary system-wide functionalities.
 */


const core::address SYSTEM = util::static_hash("system");

const std::uint32_t QUIT = util::static_hash("quit-event");

const std::uint32_t LOST_FOCUS = util::static_hash("focus-lost");

const std::uint32_t GOT_FOCUS = util::static_hash("focus-got");

} /* namespace msg */
} /* namespace zephyr */


#endif /* ZEPHYR_MESSAGES_HPP_ */
