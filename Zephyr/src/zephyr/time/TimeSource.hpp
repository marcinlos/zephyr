/**
 * @file TimeSource.hpp
 */

#ifndef ZEPHYR_TIME_TIMESOURCE_HPP_
#define ZEPHYR_TIME_TIMESOURCE_HPP_

#include <chrono>
#include <ratio>

namespace zephyr {
namespace time {

using namespace std::chrono;

/**
 * Class representing a time-telling service.
 */
class TimeSource {
private:
    typedef high_resolution_clock clock;
    clock::time_point start;

public:

    /**
     * Creates new timesource, based on @c std::chrono clock.
     */
    TimeSource()
    : start(clock::now())
    { }

    /**
     * @return Time (in seconds) since the creation of this time source
     */
    double time() const {
        auto elapsed = clock::now() - start;
        duration<double> t = duration_cast<duration<double>>(elapsed);
        return t.count();
    }

};


} /* namespace time */
} /* namespace zephyr */

#endif /* ZEPHYR_TIME_TIMESOURCE_HPP_ */
