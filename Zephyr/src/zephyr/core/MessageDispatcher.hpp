/**
 * @file MessageDispatcher.h
 */

#ifndef ZEPHYR_CORE_MESSAGEDISPATCHER_H_
#define ZEPHYR_CORE_MESSAGEDISPATCHER_H_

#include <zephyr/core/Message.hpp>
#include <unordered_map>
#include <functional>
#include <string>

namespace zephyr {
namespace core {


class MessageDispatcher {
public:
    typedef std::function<void (const Message&)> Handler;

    void registerHandler(std::uint32_t name, Handler handler);

    void dispatch(const Message& message);

private:



    std::unordered_multimap<std::uint32_t, Handler> handlers_;

};

} /* namespace core */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_MESSAGEDISPATCHER_H_ */
