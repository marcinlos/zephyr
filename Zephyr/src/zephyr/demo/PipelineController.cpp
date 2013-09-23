/**
 * @file PipelineController.cpp
 */

#include <zephyr/demo/PipelineController.hpp>
#include <zephyr/input/messages.hpp>

namespace events = zephyr::input::msg;
using zephyr::input::Key;

namespace zephyr {
namespace demo {


PipelineController::PipelineController(Renderer& renderer)
: renderer(renderer)
, uniforms(renderer.uniforms())
{ }

void PipelineController::handle(const Message& message) {
    if (message.type == events::KEYBOARD_EVENT) {
        KeyEvent e = util::any_cast<KeyEvent>(message.data);
        if (e.type == KeyEvent::Type::DOWN) {
            keyDown(e);
        }
    }
}


void PipelineController::keyDown(const KeyEvent& e) {
    if (e.key >= Key::F1 && e.key <= Key::F8) {
        int mode = static_cast<int>(e.key) - static_cast<int>(Key::F1);
        uniforms.set1i("mode", mode);
    }
}



} /* namespace demo */
} /* namespace zephyr */
