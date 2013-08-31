/**
 * @file Mod.hpp
 */

#ifndef ZEPHYR_INPUT_MOD_HPP_
#define ZEPHYR_INPUT_MOD_HPP_

#include <cstdint>
#include <iostream>
#include <stdexcept>

namespace zephyr {
namespace input {

/**
 * Modifier of the keyboard event, describing special keys held down during the
 * event.
 */
enum class Mod: std::uint16_t {
    NONE  = 0x0,
    CTRL  = 0x1,
    ALT   = 0x2,
    SHIFT = 0x4,
};

/**
 * @defgroup Mod enum operations
 */
///@{

inline Mod operator ~ (Mod mod) {
    return static_cast<Mod>(~static_cast<std::uint16_t>(mod));
}

inline Mod operator | (Mod a, Mod b) {
    uint16_t aa = static_cast<std::uint16_t>(a);
    uint16_t bb = static_cast<std::uint16_t>(b);
    return static_cast<Mod>(aa | bb);
}

inline Mod operator & (Mod a, Mod b) {
    uint16_t aa = static_cast<std::uint16_t>(a);
    uint16_t bb = static_cast<std::uint16_t>(b);
    return static_cast<Mod>(aa & bb);
}

inline Mod operator ^ (Mod a, Mod b) {
    uint16_t aa = static_cast<std::uint16_t>(a);
    uint16_t bb = static_cast<std::uint16_t>(b);
    return static_cast<Mod>(aa ^ bb);
}

inline bool has(Mod mod, Mod item) {
    return (mod & item) != Mod::NONE;
}

///@}

inline const char* to_string(Mod mod) {
    switch (mod) {
    case Mod::NONE:  return "none";
    case Mod::SHIFT: return "shift";
    case Mod::CTRL:  return "ctrl";
    case Mod::ALT:   return "alt";
    default:
        throw std::invalid_argument("Invalid mod value");
    }
}

inline std::ostream& operator << (std::ostream& os, Mod mod) {
    bool first = true;
    if (has(mod, Mod::CTRL)) {
        os << (first ? "" : "|") << to_string(Mod::CTRL);
        first = false;
    }
    if (has(mod, Mod::ALT)) {
        os << (first ? "" : "|") << to_string(Mod::ALT);
        first = false;
    }
    if (has(mod, Mod::SHIFT)) {
        os << (first ? "" : "|") << to_string(Mod::SHIFT);
        first = false;
    }
    if (first) {
        os << to_string(Mod::NONE);
    }
    return os;
}

} /* namespace input */
} /* namespace zephyr */



#endif /* ZEPHYR_INPUT_MOD_HPP_ */
