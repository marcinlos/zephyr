/**
 * @file KeyEvent.hpp
 */

#ifndef ZEPHYR_CORE_KEYEVENT_HPP_
#define ZEPHYR_CORE_KEYEVENT_HPP_

#include <zephyr/input/Key.hpp>


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

} /* namespace input */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_KEYEVENT_HPP_ */
