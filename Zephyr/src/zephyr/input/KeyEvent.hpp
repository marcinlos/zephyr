/**
 * @file KeyEvent.hpp
 */

#ifndef ZEPHYR_CORE_KEYEVENT_HPP_
#define ZEPHYR_CORE_KEYEVENT_HPP_

#include <zephyr/input/Key.hpp>
#include <zephyr/input/Mod.hpp>
#include <zephyr/util/format.hpp>
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

inline const char* to_string(KeyEvent::Type type) {
    switch (type) {
    case KeyEvent::Type::DOWN:   return "down";
    case KeyEvent::Type::UP:     return "up";
    case KeyEvent::Type::REPEAT: return "repeat";
    default:
        throw std::runtime_error("Invalid key event type");
    }
}

inline std::ostream& operator << (std::ostream& os, KeyEvent::Type type) {
    return os << to_string(type);
}

inline std::ostream& operator << (std::ostream& os, const KeyEvent& event) {
    return os << util::format("KeyEvent{key={}, {}, with={}}", event.key,
            event.type, event.mod);
}


} /* namespace input */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_KEYEVENT_HPP_ */
