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

/**
 * Synchronous message dispatcher. Receivers can register callback.
 */
class MessageDispatcher {
public:

    /** Type of the callbacks invoked during dispatching */
    typedef std::function<void (const Message&)> Handler;

    /**
     * Adds new handler callback for the specified receiver.
     *
     * @param receiver Id of the receiver category
     * @param handler Callback used to deliver the message
     */
    void registerHandler(std::uint32_t receiver, Handler handler);

    /**
     * Dispatches synchronously the specified message.
     */
    virtual void dispatch(const Message& message);

    virtual ~MessageDispatcher() = default;

private:
    /** Mapping (id -> handlers) */
    std::unordered_multimap<std::uint32_t, Handler> handlers_;

};

template <typename Class>
void registerHandler(MessageDispatcher& dispatcher, std::uint32_t receiver,
        Class* object, void (Class::*handler)(const Message&)) {
    using std::placeholders::_1;

    auto callback = std::bind(handler, object, _1);
    dispatcher.registerHandler(receiver, callback);
}

} /* namespace core */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_MESSAGEDISPATCHER_H_ */
