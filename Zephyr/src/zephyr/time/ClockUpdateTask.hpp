/**
 * @file ClockUpdateTask.hpp
 */

#ifndef ZEPHYR_TIME_CLOCKUPDATETASK_HPP_
#define ZEPHYR_TIME_CLOCKUPDATETASK_HPP_

#include <zephyr/core/Task.hpp>
#include <zephyr/time/ClockManager.hpp>


namespace zephyr {
namespace time {

/**
 * @ref Task implementation responsible for updating the clock manager.
 * Created and registered by the application root.
 */
class ClockUpdateTask: public core::Task {
public:

    /**
     * Creates task updating specified @ref ClockManager.
     *
     * @param manager
     *          @ref ClockManager to update every scheduling round
     */
    ClockUpdateTask(ClockManager& manager)
    : manager(manager)
    { }

    /**
     * Updates the clock manager.
     */
    void update() override {
        manager.update();
    }

private:

    /** Clock manager updated every round */
    ClockManager& manager;

};


} /* namespace time */
} /* namespace zephyr */

#endif /* ZEPHYR_TIME_CLOCKUPDATETASK_HPP_ */
