/**
 * @file CameraMotionBlur.hpp
 */

#ifndef ZEPHYR_EFFECTS_CAMERAMOTIONBLUR_HPP_
#define ZEPHYR_EFFECTS_CAMERAMOTIONBLUR_HPP_

#include <zephyr/input/messages.hpp>
#include <zephyr/input/InputState.hpp>
#include <zephyr/input/KeyEvent.hpp>
#include <zephyr/gfx/Renderer.hpp>
#include <array>

using zephyr::core::Message;
using zephyr::gfx::Renderer;
namespace events = zephyr::input::msg;

namespace zephyr {
namespace effects {

class CameraMotionBlur {
public:

    CameraMotionBlur(Renderer& renderer)
    : renderer(renderer)
    {
        renderer.uniforms().set1f("blurStrength", 0);
        renderer.uniforms().set1i("blurActive", false);
    }

    void handle(const Message& message) {
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


    void update(double dt) {
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

private:
    void updateUniforms(glm::vec2 blur) {
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

    input::InputState input;
    Renderer& renderer;

    bool active = false;
    glm::vec2 prevDelta;

    enum { N = 10 };

    std::array<glm::vec2, N> samples;
    int nextSample = 0;
};

} /* namespace effects */
} /* namespace zephyr */

#endif /* ZEPHYR_EFFECTS_CAMERAMOTIONBLUR_HPP_ */
