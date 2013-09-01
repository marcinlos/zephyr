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

} /* namespace input */
} /* namespace zephyr */


#endif /* ZEPHYR_INPUT_BUTTON_HPP_ */
