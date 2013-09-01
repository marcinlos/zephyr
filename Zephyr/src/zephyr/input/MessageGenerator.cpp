/**
 * InputMessageGenerator.cpp
 */

#include <zephyr/input/MessageGenerator.hpp>
#include <zephyr/input/messages.hpp>

namespace zephyr {
namespace input {

using util::Any;

MessageGenerator::MessageGenerator(core::MessageQueue& messageQueue)
: messageQueue(messageQueue)
{ }

void MessageGenerator::keyEvent(const KeyEvent& event) {
    messageQueue.post({
        msg::INPUT_SYSTEM,
        msg::KEYBOARD_EVENT,
        Any { event }
    });
}

void MessageGenerator::buttonEvent(const ButtonEvent& event) {
    messageQueue.post({
        msg::INPUT_SYSTEM,
        msg::BUTTON_EVENT,
        Any { event }
    });
}

void MessageGenerator::mouseMove(const Position& pos) {
    messageQueue.post({
        msg::INPUT_SYSTEM,
        msg::CURSOR_EVENT,
        Any { pos }
    });
}

void MessageGenerator::scroll(double dy) {
    messageQueue.post({
        msg::INPUT_SYSTEM,
        msg::SCROLL_EVENT,
        Any { dy }
    });
}

} /* namespace input */
} /* namespace zephyr */

