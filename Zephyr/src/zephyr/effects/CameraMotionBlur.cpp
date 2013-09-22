/**
 * @file CameraMotionBlur.cpp
 */

#include <zephyr/effects/CameraMotionBlur.hpp>

namespace zephyr {
namespace effects {

CameraMotionBlur::CameraMotionBlur(Renderer& renderer)
: renderer(renderer)
{
    renderer.uniforms().set1f("blurStrength", 0);
    renderer.uniforms().set1i("blurActive", false);
}

void CameraMotionBlur::handle(const Message& message) {
    using namespace zephyr::input;
    if (message.type == events::CURSOR_EVENT) {
        Position pos = util::any_cast<Position>(message.data);
        float dx =   pos.x - input.mouse().x;
        float dy = -(pos.y - input.mouse().y);

        const float treshold = 60.0f;
        prevDelta = glm::vec2 { dx, dy };
        if (glm::length2(prevDelta) > treshold) {
            active = true;
        }
        input.mouse(pos);
    }
}


void CameraMotionBlur::update(double dt) {
    samples[nextSample] = prevDelta;
    nextSample = (nextSample + 1) % N;
    prevDelta = glm::vec2(0, 0);

    const float scale = 0.02f;

    glm::vec2 blur;
    for (auto sample : samples) {
        blur += sample;
    }
    blur /= N;
    blur *= scale;

    if (active) {
        updateUniforms(blur);
    }
}

void CameraMotionBlur::updateUniforms(glm::vec2 blur) {
    float strength = glm::length2(blur);
    glm::vec2 dir = blur;

    if (strength > 1e-3f) {
        dir = glm::normalize(dir);
    } else {
        active = false;
        strength = 0.0f;
    }
    renderer.uniforms().set1f("blurStrength", strength);
    renderer.uniforms().set2f("blurDir", dir);
    renderer.uniforms().set1i("blurActive", active);
}


} /* namespace effects */
} /* namespace zephyr */
