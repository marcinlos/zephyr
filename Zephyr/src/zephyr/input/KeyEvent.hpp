/**
 * @file KeyEvent.hpp
 */

#ifndef ZEPHYR_CORE_KEYEVENT_HPP_
#define ZEPHYR_CORE_KEYEVENT_HPP_

#include <zephyr/input/Key.hpp>
#include <iostream>


namespace zephyr {
namespace input {

/**
 * Modifier of the keyboard event, describing special keys held down during the
 * event.
 */
enum Mod {
    NONE  = 0x0,
    SHIFT = 0x1,
    CTRL  = 0x2,
    ALT   = 0x4,
};

/**
 * Structure containing information about keyboard-related event. To be used
 * as the @c data member of the message structure.
 */
struct KeyEvent {

    /** What precisely happened to the said key. */
    enum class Type {
        DOWN,
        UP,
        REPEAT
    };

    Key key;
    Type type;
    Mod mod;
};


inline const char* to_string(Mod mod) {
    switch (mod) {
    case NONE: return "none";
    case SHIFT: return "shift";
    case CTRL: return "ctrl";
    case ALT: return "alt";
    default:
        throw std::invalid_argument("Invalid mod value");
    }
}

inline std::ostream& operator << (std::ostream& os, Mod mod) {
    return os << to_string(mod);
}

} /* namespace input */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_KEYEVENT_HPP_ */
