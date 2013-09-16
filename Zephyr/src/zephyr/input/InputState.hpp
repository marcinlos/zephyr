/**
 * @file InputState.hpp
 */

#ifndef ZEPHYR_INPUT_INPUTSTATE_HPP_
#define ZEPHYR_INPUT_INPUTSTATE_HPP_

#include <zephyr/input/KeyEvent.hpp>
#include <zephyr/input/ButtonEvent.hpp>
#include <zephyr/input/Position.hpp>
#include <array>


namespace zephyr {
namespace input {

class InputState {
public:

    InputState() {
        using std::begin;
        using std::end;

        std::fill(begin(keyState), end(keyState), false);
        std::fill(begin(buttonState), end(buttonState), false);
    }

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

    std::array<bool, KEY_NUMBER> keyState;
    std::array<bool, BUTTON_NUMBER> buttonState;

    Position cursorPos;
};




} /* namespace input */
} /* namespace zephyr */



#endif /* ZEPHYR_INPUT_INPUTSTATE_HPP_ */
