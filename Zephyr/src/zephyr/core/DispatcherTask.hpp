
#ifndef ZEPHYR_CORE_DISPATCHERTASK_H_
#define ZEPHYR_CORE_DISPATCHERTASK_H_

#include <zephyr/core/Task.hpp>
#include <zephyr/core/MessageQueue.hpp>
#include <zephyr/core/MessageDispatcher.hpp>


namespace zephyr {
namespace core {

/**
 * @ref Task implementation that handles message delivery.
 */
class DispatcherTask: public core::Task {
public:

    /**
     * Creates @ref DispatcherTask taking messages from the specified queue,
     * and delivering it with the specified dispatcher.
     */
    DispatcherTask(MessageQueue& queue, MessageDispatcher& dispatcher)
    : queue(queue), dispatcher(dispatcher)
    { }

    /**
     * Empties the message queue and forwards the messages to the dispatcher.
     */
    void update() override;

private:
    /** Source of the messages */
    MessageQueue& queue;

    /** Message dispatcher used to deliver messages */
    MessageDispatcher& dispatcher;

};

} /* namespace core */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_DISPATCHERTASK_H_ */
