/**
 * @file Window.hpp
 */

#ifndef ZEPHYR_GFX_WINDOW_H_
#define ZEPHYR_GFX_WINDOW_H_

#include <string>
#include <GLFW/glfw3.h>
#include <zephyr/input/InputListener.hpp>

namespace zephyr
{
namespace gfx
{

/**
 * System-level window, providing basic graphics output functionality.
 */
class Window
{
public:
    /**
     * Creates @ref Window with specified size and title displayed on the
     * titlebar.
     */
    Window(int width, int height, const std::string& title);
    
    /**
     * Releases the window resources.
     */
    ~Window();

    void setListener(input::InputListener* inputListener);

    void pollEvents() const;
    
    /**
     * Swaps graphic buffers of the window.
     */
    void swapBuffers() const;
    
private:
    /** Underlying window object */
    GLFWwindow* window_;

    /** Listener receiving information about user input */
    input::InputListener* inputListener_;


    void mouseHandler(int button, int action, int mods);

    void cursorHandler(double x, double y);

    void focusHandler(int entered);

    void scrollHandler(double dx, double dy);

    void keyHandler(int key, int scancode, int action, int mods);

    void closeHandler();

    void setupListeners();

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

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_WINDOW_H_ */
