/**
 * @file Clock.hpp
 */

#ifndef ZEPHYR_TIME_CLOCK_HPP_
#define ZEPHYR_TIME_CLOCK_HPP_

#include <tuple>

namespace zephyr {
namespace time {

/**
 * Class representing a clock.
 */
class Clock {
private:

    /**
     * Method intended to be implemented by subclasses. Provides actual values,
     * stored and exposed by this base class. This is a template method pattern
     * example.
     *
     * <p>
     * Retured value is a tuple `(current_time, dt)`. This allows custom
     * tweaking of time delta, e.g. smoothing it for the sake of more stable
     * physics.
     *
     * @param prevUpdate
     *          Time value returned by the previous call to @ref updateImpl,
     *          or 0 if the method is called the first time
     *
     * @return Tuple consisting of the current time, and the time difference
     *         between this and previous call
     */
    virtual std::tuple<double, double> updateImpl(double prevUpdate) = 0;

public:

    /**
     * @return Current time in milliseconds, measured relative to an arbitrary,
     *         but fixed and constant point in time
     */
    double time() const {
        return lastUpdate;
    }

    /**
     * @return Time difference between last two clock updates. If is is called
     * before second update, returns `0`.
     */
    double dt() const {
        return lastDt;
    }

    /**
     * Updates the clock state. Should be called once per frame.
     */
    void update() {
        std::tie(lastUpdate, lastDt) = updateImpl(lastUpdate);
    }

    /**
     * Virtual destructor, as it is polymorphic class.
     */
    virtual ~Clock() = default;

private:
    /** When was the clock last updated */
    double lastUpdate = 0.0;

    /** Last time difference as returned by @ref updateImpl */
    double lastDt = 0.0;

};

} /* namespace time */
} /* namespace zephyr */



#endif /* ZEPHYR_TIME_CLOCK_HPP_ */
