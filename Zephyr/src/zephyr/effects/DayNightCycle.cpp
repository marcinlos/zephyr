/*
 * DayNightCycle.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: los
 */
#include <zephyr/effects/DayNightCycle.hpp>

namespace zephyr {
namespace effects {


void DayNightCycle::apply(double time) {
    double t = time / dayLength;
    float theta = 2 * M_PI * t;

    glm::vec3 sunPos = calculateSunPosition(t);
    vars["sunPos"] = util::Any(sunPos);
    sunDirection = glm::normalize(-sunPos);

    float sinAbs = glm::clamp(std::sin(theta) + 0.2f, 0.0f, 1.0f);
    sunIntensity = 2.0f * sinAbs * sinAbs;
    float ambientVal = sunIntensity + 0.2f;
    float hdr = 0.7f * sunIntensity + 0.4f;//1.5f + 3.0f * sinAbs * sinAbs;


    float rem = std::fmod(t, 1);

    glm::vec3 sunCol { 1, 1, 1 };

//    if (t > 0.1f && t < 0.4f) {
//        sunCol = glm::vec3 { 1, 1, 1 };
//    } else {
//        float d = std::min(t - 0.1f, t - 0.4f);
//        sunCol.g = sunCol.b = 0;// 1 - d;
//    }

    uniforms.set1f("timeOfDay", t);
    uniforms.set3f("sunDirection", sunDirection);
    uniforms.set1f("sunIntensity", sunIntensity);
    uniforms.set3f("ambient", ambientVal * ambient);
    uniforms.set3f("sunColor", sunCol);
    uniforms.set1f("hdrMax", hdr);
}


} /* namespace effects */
} /* namespace zephyr */

