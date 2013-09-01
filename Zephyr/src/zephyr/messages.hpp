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


const core::address SYSTEM = util::static_hash("system");

const std::uint32_t QUIT = util::static_hash("quit-event");


} /* namespace msg */
} /* namespace zephyr */


#endif /* ZEPHYR_MESSAGES_HPP_ */
