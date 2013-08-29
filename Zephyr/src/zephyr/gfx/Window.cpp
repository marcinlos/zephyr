/*
 * Window.cpp
 *
 *  Created on: Jun 30, 2013
 *      Author: los
 */

#include <stdexcept>
#include <zephyr/gfx/Window.hpp>

namespace zephyr
{
namespace gfx 
{
    
Window::Window(int width, int height, const std::string& title)
{
    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_)
    {
        throw std::runtime_error("Window initialization failure");
    }
    glfwSetWindowUserPointer(window_, static_cast<void*>(this));
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::swapBuffers() const
{
    glfwSwapBuffers(window_);
}


} /* namespace gfx */
} /* namespace zephyr */