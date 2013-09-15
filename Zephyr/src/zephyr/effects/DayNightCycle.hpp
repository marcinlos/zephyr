/**
 * @file DayNightCycle.hpp
 */

#ifndef ZEPHYR_EFFECTS_DAYNIGHTCYCLE_HPP_
#define ZEPHYR_EFFECTS_DAYNIGHTCYCLE_HPP_

#include <zephyr/gfx/Renderer.hpp>


namespace zephyr {
namespace effects {

class DayNightCycle {
public:

    static constexpr float maxAngle = 50.0f * M_PI / 180;
    static constexpr float dayLength = 4.0f;

    DayNightCycle(gfx::Renderer& renderer)
    : renderer(renderer)
    , sunIntensity { 1.5f }
    , ambient { 0.2f }
    { }

public:
    void apply(double time) {
        using std::sin;
        using std::cos;
        using namespace gfx;
        double t = time / dayLength * 2 * M_PI;
        float angle = maxAngle * (cos(t));
        glm::vec3 sunPos {
            cos(angle) * cos(t),
            sin(angle),
            cos(angle) * sin(t)
        };
        sunDirection = glm::normalize(-sunPos);

        UniformPtr sunDir { new uniform3f {
            sunDirection.x,
            sunDirection.y,
            sunDirection.z
        }};
        UniformPtr sunInt { new uniform1f { sunIntensity } };
        UniformPtr ambientIntensity { new uniform1f { ambient } };

        renderer.uniforms().uniform("sunDirection", sunDir);
        renderer.uniforms().uniform("sunIntensity", sunInt);
        renderer.uniforms().uniform("ambient", ambientIntensity);
    }

private:
    gfx::Renderer& renderer;

    glm::vec3 sunDirection;
    float sunIntensity;
    float ambient;
};


} /* namespace effects */
} /* namespace zephyr */

#endif /* ZEPHYR_EFFECTS_DAYNIGHTCYCLE_HPP_ */
