/**
 * @file KeyEvent.hpp
 */

#ifndef ZEPHYR_CORE_KEYEVENT_HPP_
#define ZEPHYR_CORE_KEYEVENT_HPP_

#include <zephyr/input/Key.hpp>
#include <zephyr/input/Mod.hpp>
#include <iostream>


namespace zephyr {
namespace input {

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
