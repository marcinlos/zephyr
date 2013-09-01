/**
 * @file InputListener.hpp
 */

#ifndef ZEPHYR_CORE_INPUTLISTENER_HPP_
#define ZEPHYR_CORE_INPUTLISTENER_HPP_

#include <zephyr/input/KeyEvent.hpp>
#include <zephyr/input/ButtonEvent.hpp>

namespace zephyr {
namespace input {

/**
 * Interface for receiving input events directly from the actual source, e.g.
 * window or device drivers.
 */
class InputListener {
public:

    /**
     * Invoked when keyboard key is pressed/released.
     */
    virtual void keyEvent(const KeyEvent& e) = 0;

    /**
     * Invoked when mouse button is pressed/released.
     */
    virtual void buttonEvent(const ButtonEvent& e) = 0;

    /**
     * Invoked when the mouse position changes.
     */
    virtual void mouseMove(const Position& pos) = 0;

    /**
     * Invoked when the mouse wheel is used.
     */
    virtual void scroll(double dy) = 0;

    virtual ~InputListener() = default;

};


} /* namespace input */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_INPUTLISTENER_HPP_ */
