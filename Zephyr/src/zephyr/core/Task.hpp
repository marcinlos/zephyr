/**
 * @file Task.hpp
 */

#ifndef ZEPHYR_CORE_TASK_HPP_
#define ZEPHYR_CORE_TASK_HPP_

#include <memory>

namespace zephyr {
namespace core {

/** Pointer type holding the task */
typedef std::shared_ptr<class Task> TaskPtr;

/**
 * Base class for tasks scheduled for execution in the main loop. Each
 * iteration, `update()` is called.
 */
class Task {
public:

    /**
     * Invoked when the task is added to task scheduler.
     */
    virtual void start();

    /**
     * Invoked when the task is about to be removed from the active task
     * collection.
     */
    virtual void stop();

    /**
     * Invoked when the task is about to be suspended.
     */
    virtual void suspend();

    /**
     * Invoked when the task is resumed after being suspended.
     */
    virtual void resume();

    /**
     * Invoked in the main loop when the task is active.
     */
    virtual void update() = 0;

    /**
     * Virtual destructor, since the class is designed to be used as a
     * polymorphic base.
     */
    virtual ~Task() = 0;

};

} /* namespace core */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_TASK_HPP_ */
