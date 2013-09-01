/**
 * @file InputListener.hpp
 */

#ifndef ZEPHYR_CORE_INPUTLISTENER_HPP_
#define ZEPHYR_CORE_INPUTLISTENER_HPP_

#include <zephyr/input/KeyEvent.hpp>
#include <zephyr/input/ButtonEvent.hpp>

namespace zephyr {
namespace input {

class InputListener {
public:

    virtual void keyEvent(const KeyEvent& e) = 0;

    virtual void buttonEvent(const ButtonEvent& e) = 0;

    virtual void mouseMove(const Position& pos) = 0;

    virtual void scroll(double dy) = 0;

    virtual ~InputListener() = 0;

};

inline InputListener::~InputListener() {
    // empty
}


} /* namespace input */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_INPUTLISTENER_HPP_ */
