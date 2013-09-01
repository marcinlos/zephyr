/**
 * InputMessageGenerator.cpp
 */

#include <zephyr/input/InputMessageGenerator.hpp>

namespace zephyr {
namespace input {

InputMessageGenerator::InputMessageGenerator(core::MessageQueue& messageQueue)
: messageQueue(messageQueue)
{ }

void InputMessageGenerator::keyEvent(const KeyEvent& e) {
//    messageQueue.pos
}

void InputMessageGenerator::buttonEvent(const ButtonEvent& e) {

}

void InputMessageGenerator::mouseMove(const Position& pos) {

}

void InputMessageGenerator::scroll(double dy) {

}

} /* namespace input */
} /* namespace zephyr */


