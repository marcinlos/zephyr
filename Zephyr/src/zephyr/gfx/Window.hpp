/*
 * Window.hpp
 *
 *  Created on: Jun 30, 2013
 *      Author: los
 */

#ifndef ZEPHYR_GFX_WINDOW_H_
#define ZEPHYR_GFX_WINDOW_H_

#include <string>
#include <GLFW/glfw3.h>

namespace zephyr
{
namespace gfx
{
    
class Window
{
public:
    Window(int width, int height, const std::string& title);
    
    void pollEvents();
    
    void swapBuffers() const;
    
private:
    GLFWwindow* window_; 
};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_WINDOW_H_ */
