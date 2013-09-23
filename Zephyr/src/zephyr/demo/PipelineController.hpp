/**
 * @file PipelineController.hpp
 */

#ifndef ZEPHYR_DEMO_PIPELINECONTROLLER_HPP_
#define ZEPHYR_DEMO_PIPELINECONTROLLER_HPP_

#include <zephyr/gfx/Renderer.hpp>
#include <zephyr/input/InputState.hpp>
#include <zephyr/core/Message.hpp>
#include <zephyr/input/KeyEvent.hpp>
#include <zephyr/util/Any.hpp>


using zephyr::gfx::Renderer;
using zephyr::gfx::UniformManager;
using zephyr::input::InputState;
using zephyr::core::Message;
using zephyr::input::KeyEvent;


namespace zephyr {
namespace demo {

class PipelineController {
public:

    PipelineController(Renderer& renderer);

    void handle(const Message& message);

private:

    void keyDown(const KeyEvent& e);

    InputState input;
    Renderer& renderer;
    UniformManager& uniforms;
};

} /* namespace demo */
} /* namespace zephyr */

#endif /* ZEPHYR_DEMO_PIPELINECONTROLLER_HPP_ */
