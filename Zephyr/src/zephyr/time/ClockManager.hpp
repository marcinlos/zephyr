/**
 * @file ClockManager.hpp
 */

#ifndef ZEPHYR_TIME_CLOCKMANAGER_HPP_
#define ZEPHYR_TIME_CLOCKMANAGER_HPP_

#include <zephyr/time/TimeSource.hpp>
#include <zephyr/time/Clock.hpp>
#include <zephyr/time/DefaultClock.hpp>
#include <memory>


namespace zephyr {
namespace time {


class ClockManager {
public:

    ClockManager()
    : mainClock(timeSource)
    { }

    /**
     * @return Main clock instance
     */
    const Clock& getMainClock() const {
        return mainClock;
    }

    void update() {
        mainClock.update();
    }

private:

    TimeSource timeSource;

    /** Main clock instance */
    DefaultClock<TimeSource> mainClock;



};

} /* namespace time */
} /* namespace zephyr */

#endif /* ZEPHYR_TIME_CLOCKMANAGER_HPP_ */
