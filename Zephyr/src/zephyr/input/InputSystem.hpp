/**
 * @file InputSystem.hpp
 */

#ifndef ZEPHYR_INPUT_INPUTSYSTEM_HPP_
#define ZEPHYR_INPUT_INPUTSYSTEM_HPP_

#include <zephyr/Context.hpp>
#include <zephyr/core/Message.hpp>
#include <zephyr/input/KeyEvent.hpp>
#include <zephyr/util/Any.hpp>
#include <zephyr/input/messages.hpp>
#include <zephyr/messages.hpp>
#include <functional>


namespace zephyr {
namespace input {


class InputSystem {
private:

    core::MessageQueue& queue;

public:

    InputSystem(Context ctx)
    : queue(ctx.messageQueue) {
        using std::placeholders::_1;
        core::Handler handler = std::bind(&InputSystem::message, this, _1);
        ctx.dispatcher.registerHandler(msg::INPUT_SYSTEM, handler);
    }

    void message(const core::Message& message) {
        std::cout << message.data << std::endl;
        if (message.type == msg::KEYBOARD_EVENT) {
            KeyEvent e = util::any_cast<KeyEvent>(message.data);
            if (e.key == Key::ESCAPE && e.type == KeyEvent::Type::DOWN) {
                queue.post({
                    zephyr::msg::SYSTEM,
                    zephyr::msg::QUIT,
                    util::Any {}
                });
            }
        }
    }

private:

};

} /* namespace input */
} /* namespace zephyr */

#endif /* ZEPHYR_INPUT_INPUTSYSTEM_HPP_ */
