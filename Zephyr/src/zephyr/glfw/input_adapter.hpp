/*
 * @file input_adapter.hpp
 *
 * Bridge between GLFW input definitions and the project-specific ones.
 */

#ifndef ZEPHYR_GLFW_INPUT_ADAPTER_HPP_
#define ZEPHYR_GLFW_INPUT_ADAPTER_HPP_

#include <GLFW/glfw3.h>
#include <zephyr/input/Mod.hpp>
#include <zephyr/input/Key.hpp>
#include <zephyr/input/Button.hpp>
#include <zephyr/input/KeyEvent.hpp>
#include <zephyr/input/ButtonEvent.hpp>
#include <zephyr/input/Position.hpp>
#include <stdexcept>

using zephyr::input::Mod;
using zephyr::input::Key;
using zephyr::input::Button;
using zephyr::input::KeyEvent;
using zephyr::input::ButtonEvent;
using zephyr::input::Position;

namespace zephyr {
namespace glfw {

/**
 * Converts input event modifier from GLFW raw integer to zephyr's specific
 * enumeration.
 */
inline Mod modFromGLFW(int mod) {
    Mod value = Mod::NONE;
    if (mod & GLFW_MOD_ALT) {
        value = value | Mod::ALT;
    }
    if (mod & GLFW_MOD_CONTROL) {
        value = value | Mod::CTRL;
    }
    if (mod & GLFW_MOD_SHIFT) {
        value = value | Mod::SHIFT;
    }
    return value;
}

/**
 * @defgroup Keyboard event conversion
 */
/// @{

inline KeyEvent::Type keyEventTypeFromGLFW(int type) {
    switch (type) {
    case GLFW_PRESS:   return KeyEvent::Type::DOWN;
    case GLFW_RELEASE: return KeyEvent::Type::UP;
    case GLFW_REPEAT:  return KeyEvent::Type::REPEAT;
    default:
        throw std::runtime_error("Invalid key action");
    }
}

Key keyFromGLFW(int key);

inline KeyEvent keyEventFromGLFW(int key, int action, int mods) {
    Key k = keyFromGLFW(key);
    KeyEvent::Type t = keyEventTypeFromGLFW(action);
    Mod m = modFromGLFW(mods);
    return {k, t, m};
}

/// @}


/**
 * @defgroup Mouse event conversion
 */
/// @{

inline ButtonEvent::Type buttonEventTypeFromGLFW(int type) {
    switch (type) {
    case GLFW_PRESS:   return ButtonEvent::Type::DOWN;
    case GLFW_RELEASE: return ButtonEvent::Type::UP;
    default:
        throw std::runtime_error("Invalid button action");
    }
}

inline Button buttonFromGLFW(int button) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:   return Button::LEFT;
    case GLFW_MOUSE_BUTTON_RIGHT:  return Button::RIGHT;
    case GLFW_MOUSE_BUTTON_MIDDLE: return Button::MIDDLE;
    default: return Button::UNKNOWN;
    }
}

inline ButtonEvent buttonEventFromGLFW(int button, int action, int mods,
        const Position& pos) {
    Button b = buttonFromGLFW(button);
    ButtonEvent::Type t = buttonEventTypeFromGLFW(action);
    Mod m = modFromGLFW(mods);
    return {b, t, pos, m};
}

/// @}


} /* namespace glfw */
} /* namespace zephyr */

#endif /* ZEPHYR_GLFW_INPUT_ADAPTER_HPP_ */
