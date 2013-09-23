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
using zephyr::input::InputState;
namespace events = zephyr::input::msg;

namespace zephyr {
namespace effects {

class CameraMotionBlur {
public:

    CameraMotionBlur(Renderer& renderer);

    void handle(const Message& message);

    void update(double dt);

private:
    void updateUniforms(glm::vec2 blur);

    InputState input;
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
