/**
 * @file Key.hpp
 *
 * Definition of keyboard key enum.
 */

#ifndef ZEPHYR_INPUT_KEY_HPP_
#define ZEPHYR_INPUT_KEY_HPP_

#include <iostream>

namespace zephyr {
namespace input {

/**
 * Enum type representing aribitrarily choosen, albeit fairly complete subset
 * of keys on the keyboard.
 */
enum class Key {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    K0,
    K1,
    K2,
    K3,
    K4,
    K5,
    K6,
    K7,
    K8,
    K9,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    LEFT_CTRL,
    RIGHT_CTRL,
    LEFT_ALT,
    RIGHT_ALT,
    LEFT_SHIFT,
    RIGHT_SHIFT,
    MENU,
    SPACE,
    TAB,
    CAPS_LOCK,
    ESCAPE,
    RETURN,
    BACKSPACE,
    COMMA,
    PERIOD,
    APOSTROPHE,
    SLASH,
    BACKSLASH,
    GRAVE,
    SEMICOLON,
    RIGHT_BRACKET,
    LEFT_BRACKET,
    MINUS,
    EQUALS,
    INSERT,
    DELETE,
    HOME,
    END,
    PAGE_UP,
    PAGE_DOWN,
    PRINT_SCREEN,
    SCROLL_LOCK,
    NUM_LOCK,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    PAD0,
    PAD1,
    PAD2,
    PAD3,
    PAD4,
    PAD5,
    PAD6,
    PAD7,
    PAD8,
    PAD9,
    PAD_UP,
    PAD_DOWN,
    PAD_LEFT,
    PAD_RIGHT,
    PAD_PLUS,
    PAD_MINUS,
    PAD_TIMES,
    PAD_DIV,
    PAD_DECIMAL,
    PAD_PAGE_DOWN,

    UNKNOWN,
    LAST = UNKNOWN
};

constexpr const char* to_string(Key key);

inline std::ostream& operator << (std::ostream& os, Key key) {
    return os << to_string(key);
}

} /* namespace input */
} /* namespace zephyr */


#endif /* ZEPHYR_INPUT_KEY_HPP_ */
