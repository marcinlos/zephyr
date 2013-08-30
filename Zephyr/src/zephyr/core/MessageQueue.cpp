#include <zephyr/core/MessageQueue.hpp>

namespace zephyr {
namespace core {

void MessageQueue::post(const Message& message) {
    std::lock_guard<std::mutex> guard(mutex_);
    messages_.push(message);
}

bool MessageQueue::empty() const {
    std::lock_guard<std::mutex> guard(mutex_);
    return messages_.empty();
}

Message MessageQueue::pop() {
    std::lock_guard<std::mutex> guard(mutex_);
    return popMessageNoLock();
}


Message MessageQueue::popMessageNoLock() {
    Message message = messages_.front();
    messages_.pop();
    return message;
}


} /* namespace core */
} /* namespace zephyr */
