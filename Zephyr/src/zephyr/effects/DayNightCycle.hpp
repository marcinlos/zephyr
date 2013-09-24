/**
 * @file DayNightCycle.hpp
 */

#ifndef ZEPHYR_EFFECTS_DAYNIGHTCYCLE_HPP_
#define ZEPHYR_EFFECTS_DAYNIGHTCYCLE_HPP_

#include <zephyr/Root.hpp>
#include <zephyr/gfx/Renderer.hpp>

using zephyr::core::Register;
using zephyr::gfx::Renderer;
using zephyr::gfx::UniformManager;

namespace zephyr {
namespace effects {

class DayNightCycle {
public:

    static constexpr float dayLength = 60.0f;

    DayNightCycle(Root& root)
    : root(root)
    , vars(root.vars())
    , renderer(root.graphics().renderer())
    , uniforms(renderer.uniforms())
    , sunIntensity { 1 }
    , ambient { 0.1f, 0.1f, 0.1f }
    {
        vars.put("sunPos", glm::vec3 {});
    }

public:

    glm::vec3 calculateSunPosition(float t) {
        glm::vec4 pos { 1, 0, 0, 1 };
        pos = glm::rotate<float>(360 * t, 0, 0, 1) * pos;
        pos = glm::rotate<float>(-50, 1, 0, 0) * pos;
        return glm::vec3 { pos };
    }

    void apply(double time);

private:
    Root& root;
    Register& vars;
    Renderer& renderer;
    UniformManager& uniforms;

    glm::vec3 sunDirection;
    float sunIntensity;
    glm::vec3 ambient;
};


} /* namespace effects */
} /* namespace zephyr */

#endif /* ZEPHYR_EFFECTS_DAYNIGHTCYCLE_HPP_ */
