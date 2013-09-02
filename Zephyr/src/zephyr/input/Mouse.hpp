/**
 * @file Button.hpp
 */

#ifndef ZEPHYR_INPUT_BUTTON_HPP_
#define ZEPHYR_INPUT_BUTTON_HPP_

#include <iostream>
#include <stdexcept>

namespace zephyr {
namespace input {

/**
 * Enumeration representing mouse buttons.
 */
enum class Button {
    LEFT,
    RIGHT,
    MIDDLE,

    UNKNOWN
};

/**
 * Enumeration representing mouse operation mode - position-based and
 * movement-based.
 */
enum class MouseMode {
    ABSOLUTE,
    RELATIVE,
};


inline const char* to_string(Button button) {
    switch (button) {
    case Button::LEFT:    return "left";
    case Button::RIGHT:   return "right";
    case Button::MIDDLE:  return "middle";
    case Button::UNKNOWN: return "?";
    default:
        throw std::runtime_error("Invalid mouse button");
    }
}

inline std::ostream& operator << (std::ostream& os, Button button) {
    return os << to_string(button);
}


inline const char* to_string(MouseMode mode) {
    switch (mode) {
    case MouseMode::ABSOLUTE: return "absolute";
    case MouseMode::RELATIVE: return "relative";
    default:
            throw std::runtime_error("Invalid mouse operation mode");
    }
}

inline std::ostream& operator << (std::ostream& os, MouseMode mode) {
    return os << to_string(mode);
}

} /* namespace input */
} /* namespace zephyr */


#endif /* ZEPHYR_INPUT_BUTTON_HPP_ */
