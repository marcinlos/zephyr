/**
 * @file MessageQueue.hpp
 */

#ifndef ZEPHYR_CORE_MESSAGEQUEUE_HPP_
#define ZEPHYR_CORE_MESSAGEQUEUE_HPP_

#include <zephyr/core/Message.hpp>
#include <queue>
#include <mutex>


namespace zephyr {
namespace core {

/**
 *
 */
class MessageQueue {
public:

    void post(const Message& message);

    bool empty() const;

    Message pop();

    /**
     * Atomically empties the queue.
     */
    template <typename OutputIter>
    void drain(OutputIter iter) {
        std::lock_guard<std::mutex> guard(mutex_);
        while (! messages_.empty()) {
            *iter++ = popMessageNoLock();
        }
    }


private:

    /** Removes the message from the fron of the queue */
    Message popMessageNoLock();

    /** Queue holding the messages */
    std::queue<Message> messages_;

    /** Mutex protecting the queue */
    mutable std::mutex mutex_;

};

} /* namespace core */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_MESSAGEQUEUE_HPP_ */
