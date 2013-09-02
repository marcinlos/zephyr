/**
 * @file Window.hpp
 */

#ifndef ZEPHYR_WINDOW_WINDOW_H_
#define ZEPHYR_WINDOW_WINDOW_H_

#include <zephyr/input/InputListener.hpp>
#include <zephyr/input/Position.hpp>
#include <zephyr/core/MessageQueue.hpp>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

namespace zephyr {
namespace window {

using core::MessageQueue;

using input::MouseMode;
using input::Position;
using input::InputListener;


/** Type of receiver of input messages coming from the window */
typedef std::shared_ptr<InputListener> ListenerPtr;

/**
 * Initialization data for the window.
 */
struct InitInfo {
    int width;
    int height;
    std::string title;
    bool fullscreen;
};

/**
 * Deleter for GLFWwindow pointer, intended to be used with smartpointers to
 * provide necessary custom release semantics.
 */
struct window_deleter {
    void operator () (GLFWwindow* window) {
        glfwDestroyWindow(window);
    }
};


/**
 * System-level window, providing basic graphics output functionality and
 * receiving and propagating input events related to keyboard and mouse.
 */
class Window
{
public:
    /**
     * Creates @ref Window with specified size and title displayed on the
     * titlebar.
     *
     * @param [in] info
     *          Initialization window parameters
     *
     * @param [in] queue
     *          Message queue used for internal engine communication
     */
    Window(const InitInfo& info, MessageQueue& queue);

    /**
     * Sets the listener receiving information about input events.
     *
     * @param [in] inputListener
     *          Listener to notify about events
     */
    void setListener(const ListenerPtr& inputListener);
    
    /**
     * Enables/disables fullscreen mode.
     *
     * @param [in] full
     *          Whether the fullscreen mode should be enabled or disabled
     */
    void fullscreen(bool full);

    /**
     * Changes the display mode between fullscreen and ordinary windowed mode.
     *
     * @return Previous mode
     */
    bool toggleFullscreen();

    /**
     * Sets the mouse operation mode.
     *
     * @param [in] mode
     *          Mode to set
     */
    void mouseMode(MouseMode mode);

    /**
     * Polls the underlying library, so that the events are delivered through
     * the previously set callbacks.
     */
    void pollEvents() const;
    
    /**
     * Swaps graphic buffers of the window. Should be invoked at the end of
     * each frame.
     */
    void swapBuffers() const;

private:
    typedef std::unique_ptr<GLFWwindow, window_deleter> WindowPtr;

    /** Underlying window object */
    WindowPtr window_;

    /** Message queue used for internal communication */
    MessageQueue& queue;

    /** Listener receiving information about user input */
    ListenerPtr inputListener_;

    bool fullscreen_;

    MouseMode mouseMode_;

    std::string title_;

    /**
     * State of the window saved before going fullscreen.
     */
    struct SavedState {
        int width;
        int height;

    } state_;


    void setupListeners(GLFWwindow* window);

    WindowPtr createWindow(const InitInfo& info);

    Position getCursorPosition();

    void enableFullscren();

    void disableFullscreen();


    void mouseHandler(int button, int action, int mods);

    void cursorHandler(double x, double y);

    void focusHandler(int entered);

    void scrollHandler(double dx, double dy);

    void keyHandler(int key, int scancode, int action, int mods);

    void closeHandler();


    /**
     * Helper function, extracts @ref Window pointer from GLFWwindow.
     */
    static Window* getWindow(GLFWwindow* window);

    /**
     * Static member function used as mouse button press/release events callback.
     */
    static void mouseHandler(GLFWwindow* window, int button, int action,
            int mods);

    /**
     * Static member function used as mouse move callback.
     */
    static void cursorHandler(GLFWwindow* window, double x, double y);

    /**
     * Static member function used as callback for events caused by cursor
     * entering/leaving the window.
     */
    static void focusHandler(GLFWwindow* window, int entered);

    /**
     * Static member function used as callback for scrolling events.
     */
    static void scrollHandler(GLFWwindow* window, double dx, double dy);

    /**
     * Static member function used as callback for keyboard events.
     */
    static void keyHandler(GLFWwindow* window, int key, int scancode,
            int action, int mods);

    /**
     * Static member function used as callback for closing the window.
     */
    static void closeHandler(GLFWwindow* window);

};

} /* namespace window */
} /* namespace zephyr */

#endif /* ZEPHYR_WINDOW_WINDOW_H_ */
