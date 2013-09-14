/**
 * @file DayNightCycle.hpp
 */

#ifndef ZEPHYR_EFFECTS_DAYNIGHTCYCLE_HPP_
#define ZEPHYR_EFFECTS_DAYNIGHTCYCLE_HPP_

namespace zephyr {
namespace effects {

class DayNightCycle {


private:
    glm::vec3 sunDirection;
    float sunIntensity;
    float ambient;
};


} /* namespace effects */
} /* namespace zephyr */

#endif /* ZEPHYR_EFFECTS_DAYNIGHTCYCLE_HPP_ */
