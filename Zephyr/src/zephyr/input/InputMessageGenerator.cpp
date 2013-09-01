/**
 * InputMessageGenerator.cpp
 */

#include <zephyr/input/InputMessageGenerator.hpp>
#include <zephyr/input/messages.hpp>

namespace zephyr {
namespace input {

using util::Any;

InputMessageGenerator::InputMessageGenerator(core::MessageQueue& messageQueue)
        : messageQueue(messageQueue) {
}

void InputMessageGenerator::keyEvent(const KeyEvent& event) {
    core::Message message {
        msg::INPUT_SYSTEM,
        msg::KEYBOARD_EVENT,
        Any { event }
    };
    messageQueue.post(message);
}

void InputMessageGenerator::buttonEvent(const ButtonEvent& event) {
    core::Message message {
        msg::INPUT_SYSTEM,
        msg::BUTTON_EVENT,
        Any { event }
    };
    messageQueue.post(message);
}

void InputMessageGenerator::mouseMove(const Position& pos) {

}

void InputMessageGenerator::scroll(double dy) {

}

} /* namespace input */
} /* namespace zephyr */

