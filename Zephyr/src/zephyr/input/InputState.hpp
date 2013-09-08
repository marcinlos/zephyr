/**
 * @file InputState.hpp
 */

#ifndef ZEPHYR_INPUT_INPUTSTATE_HPP_
#define ZEPHYR_INPUT_INPUTSTATE_HPP_

#include <zephyr/input/KeyEvent.hpp>
#include <zephyr/input/ButtonEvent.hpp>
#include <zephyr/input/Position.hpp>


namespace zephyr {
namespace input {

class InputState {
public:

    bool& operator [] (Key key) {
        return keyState[asInt(key)];
    }

    bool operator [] (Key key) const {
        return keyState[asInt(key)];
    }

    bool& operator [] (Button button) {
        return buttonState[asInt(button)];
    }

    bool operator [] (Button button) const {
        return buttonState[asInt(button)];
    }

    Position mouse() const {
        return cursorPos;
    }

    void mouse(Position position) {
        cursorPos = position;
    }


private:
    int asInt(Key key) const {
        return static_cast<int>(key);
    }

    int asInt(Button button) const {
        return static_cast<int>(button);
    }

    const static int KEY_NUMBER = static_cast<int>(Key::LAST);
    const static int BUTTON_NUMBER = static_cast<int>(Button::LAST);

    bool keyState[KEY_NUMBER];
    bool buttonState[BUTTON_NUMBER];

    Position cursorPos;
};




} /* namespace input */
} /* namespace zephyr */



#endif /* ZEPHYR_INPUT_INPUTSTATE_HPP_ */
