/**
 * @file InputSystem.hpp
 */

#ifndef ZEPHYR_INPUT_INPUTSYSTEM_HPP_
#define ZEPHYR_INPUT_INPUTSYSTEM_HPP_

#include <zephyr/Context.hpp>
#include <zephyr/core/Message.hpp>
#include <zephyr/input/messages.hpp>
#include <functional>


namespace zephyr {
namespace input {


class InputSystem {
public:

    InputSystem(Context ctx) {
        using std::placeholders::_1;
        core::Handler handler = std::bind(&InputSystem::message, this, _1);
        ctx.dispatcher.registerHandler(msg::INPUT_SYSTEM, handler);
    }

    void message(const core::Message& message) {
        std::cout << message.data << std::endl;
    }

private:

};

} /* namespace input */
} /* namespace zephyr */

#endif /* ZEPHYR_INPUT_INPUTSYSTEM_HPP_ */
