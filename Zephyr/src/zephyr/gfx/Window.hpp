/**
 * @file Window.hpp
 */

#ifndef ZEPHYR_GFX_WINDOW_H_
#define ZEPHYR_GFX_WINDOW_H_

#include <string>
#include <GLFW/glfw3.h>

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
    
    void pollEvents();
    
    /**
     * Swaps graphic buffers of the window.
     */
    void swapBuffers() const;
    
private:
    GLFWwindow* window_; 
};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_WINDOW_H_ */
