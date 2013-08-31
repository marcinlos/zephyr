
#include <zephyr/core/DispatcherTask.hpp>
#include <vector>
#include <iterator>

namespace zephyr {
namespace core {

void DispatcherTask::update() {
    std::vector<Message> messages;
    queue.drain(back_inserter(messages));

    for (const Message& message : messages) {
        dispatcher.dispatch(message);
    }
}

} /* namespace core */
} /* namespace zephyr */
