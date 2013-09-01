/**
 * @file InputMessagePoster.hpp
 */

#ifndef ZEPHYR_INPUT_INPUTMESSAGEPOSTER_HPP_
#define ZEPHYR_INPUT_INPUTMESSAGEPOSTER_HPP_

#include <zephyr/input/InputListener.hpp>
#include <zephyr/core/MessageQueue.hpp>
#include <zephyr/input/messages.hpp>


namespace zephyr {
namespace input {

class MessageGenerator: public InputListener {
public:

    MessageGenerator(core::MessageQueue& messageQueue);

    void keyEvent(const KeyEvent& e) override;

    void buttonEvent(const ButtonEvent& e) override;

    void mouseMove(const Position& pos) override;

    void scroll(double dy) override;


private:
    core::MessageQueue& messageQueue;
};

} /* namespace input */
} /* namespace zephyr */

#endif /* ZEPHYR_INPUT_INPUTMESSAGEPOSTER_HPP_ */
