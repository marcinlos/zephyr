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
#include <zephyr/input/KeyEvent.hpp>
#include <stdexcept>

using zephyr::input::Mod;
using zephyr::input::KeyEvent;
using zephyr::input::Key;

namespace zephyr {
namespace glfw {

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

inline KeyEvent::Type eventTypeFromGLFW(int type) {
    switch (type) {
    case GLFW_PRESS:   return KeyEvent::Type::DOWN;
    case GLFW_RELEASE: return KeyEvent::Type::UP;
    case GLFW_REPEAT:  return KeyEvent::Type::REPEAT;
    default:
        throw std::runtime_error("Invalid key action");
    }
}

Key keyFromGLFW(int key);

inline KeyEvent eventFromGLFW(int key, int eventType, int mod) {
    Key k = keyFromGLFW(key);
    KeyEvent::Type t = eventTypeFromGLFW(eventType);
    Mod m = modFromGLFW(mod);
    return {k, t, m};
}


} /* namespace glfw */
} /* namespace zephyr */

#endif /* ZEPHYR_GLFW_INPUT_ADAPTER_HPP_ */
