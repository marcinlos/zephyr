/**
 * @file DayNightCycle.hpp
 */

#ifndef ZEPHYR_EFFECTS_DAYNIGHTCYCLE_HPP_
#define ZEPHYR_EFFECTS_DAYNIGHTCYCLE_HPP_

#include <zephyr/Root.hpp>
#include <zephyr/gfx/Renderer.hpp>


namespace zephyr {
namespace effects {

class DayNightCycle {
public:

    static constexpr float dayLength = 60.0f;

    DayNightCycle(Root& root)
    : root(root)
    , renderer(root.graphics().renderer())
    , sunIntensity { 1 }
    , ambient { 0.1f, 0.1f, 0.1f }
    {
        root.vars().put("sunPos", glm::vec3 {});
    }

public:

    glm::vec3 calculateSunPosition(float t) {
        glm::vec4 pos { 1, 0, 0, 1 };
        pos = glm::rotate<float>(360 * t, 0, 0, 1) * pos;
        pos = glm::rotate<float>(-50, 1, 0, 0) * pos;
        return glm::vec3 { pos };
    }

    void apply(double time) {
        using namespace gfx;
        double t = time / dayLength;
        float theta = 2 * M_PI * t;

        glm::vec3 sunPos = calculateSunPosition(t);
        root.vars()["sunPos"] = util::Any(sunPos);
        sunDirection = glm::normalize(-sunPos);
        glm::vec3 sunCol { 1, 1, 1 };

        float sinAbs = glm::clamp(std::sin(theta) + 0.2f, 0.0f, 1.0f);
        sunIntensity = 2.0f * sinAbs * sinAbs;
        float ambientVal = sunIntensity + 0.4f;
        float hdr = 0.7f * sunIntensity + 0.4f;//1.5f + 3.0f * sinAbs * sinAbs;

        UniformPtr sunDir { new uniform3f {
            sunDirection.x,
            sunDirection.y,
            sunDirection.z
        }};
        UniformPtr sunInt { new uniform1f { sunIntensity } };
        UniformPtr ambientUnif { new uniform3f {
            ambientVal * ambient.x,
            ambientVal * ambient.y,
            ambientVal * ambient.z
        } };

        UniformPtr sunColor { new uniform3f {
            sunCol.x,
            sunCol.y,
            sunCol.z
        } };

        UniformPtr hdrMax { new uniform1f { hdr } };

        renderer.uniforms().uniform("sunDirection", sunDir);
        renderer.uniforms().uniform("sunIntensity", sunInt);
        renderer.uniforms().uniform("ambient", ambientUnif);
        renderer.uniforms().uniform("sunColor", sunColor);
        renderer.uniforms().uniform("hdrMax", hdrMax);
    }

private:
    Root& root;
    gfx::Renderer& renderer;

    glm::vec3 sunDirection;
    float sunIntensity;
    glm::vec3 ambient;
};


} /* namespace effects */
} /* namespace zephyr */

#endif /* ZEPHYR_EFFECTS_DAYNIGHTCYCLE_HPP_ */
