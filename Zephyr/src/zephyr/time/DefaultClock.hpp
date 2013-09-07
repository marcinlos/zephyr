/**
 * @file DefaultClock.hpp
 */

#ifndef ZEPHYR_TIME_DEFAULTCLOCK_HPP_
#define ZEPHYR_TIME_DEFAULTCLOCK_HPP_

#include <zephyr/time/Clock.hpp>

namespace zephyr {
namespace time {

template <typename TimeSource>
class DefaultClock: public Clock {
private:

    const TimeSource& source;

    std::tuple<double, double> updateImpl(double prevUpdate) override {
        double time = source.time();
        double dt = time - prevUpdate;
        return std::make_tuple(time, dt);
    }

public:

    DefaultClock(const TimeSource& source)
    : source(source)
    { }

};


} /* namespace time */
} /* namespace zephyr */

#endif /* ZEPHYR_TIME_DEFAULTCLOCK_HPP_ */
