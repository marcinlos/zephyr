/*
 * MessageDispatcher.cpp
 *
 *  Created on: Aug 29, 2013
 *      Author: los
 */

#include <zephyr/core/MessageDispatcher.hpp>

namespace zephyr {
namespace core {

void MessageDispatcher::registerHandler(std::uint32_t name, Handler handler) {
    handlers_.emplace(name, handler);
}

void MessageDispatcher::dispatch(const Message& message) {
    typedef std::pair<std::uint32_t, Handler> pair;

    auto range = handlers_.equal_range(message.target);
    std::for_each(range.first, range.second, [&message](pair p) {
        p.second(message);
    });
}

} /* namespace core */
} /* namespace zephyr */
