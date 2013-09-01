/**
 * @file MouseEvent.hpp
 */

#ifndef ZEPHYR_INPUT_MOUSEEVENT_HPP_
#define ZEPHYR_INPUT_MOUSEEVENT_HPP_

#include <zephyr/input/Mod.hpp>
#include <zephyr/input/Button.hpp>
#include <zephyr/input/Position.hpp>
#include <zephyr/util/format.hpp>
#include <iostream>
#include <stdexcept>


namespace zephyr {
namespace input {

/**
 * Structure carrying information about mouse press/release event.
 */
struct ButtonEvent {

    /** What precisely happened */
    enum class Type {
        UP,
        DOWN,
    };

    Button button;
    Type type;
    Position pos;
    Mod mod;
};

inline const char* to_string(ButtonEvent::Type type) {
    switch (type) {
    case ButtonEvent::Type::UP:   return "up";
    case ButtonEvent::Type::DOWN: return "down";
    default:
        throw std::runtime_error("Invalid key event type");
    }
}

inline std::ostream& operator << (std::ostream& os, ButtonEvent::Type type) {
    return os << to_string(type);
}


inline std::ostream& operator << (std::ostream& os, const ButtonEvent& event) {
    return os << util::format("MouseEvent{button={}, {} at={}, with={}}",
            event.button, event.type, event.pos, event.mod);
}

} /* namespace input */
} /* namespace zephyr */

#endif /* ZEPHYR_INPUT_MOUSEEVENT_HPP_ */
