#include <stdexcept>
#include <zephyr/gfx/Window.hpp>
#include <iostream>

namespace zephyr {
namespace gfx {

using input::KeyEvent;
using input::Key;
using input::Mod;


Window::Window(int width, int height, const std::string& title)
: inputListener_(nullptr)
{
    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_) {
        throw std::runtime_error("Window initialization failure");
    }
    glfwSetWindowUserPointer(window_, this);
    setupListeners();
}

Window::~Window() {
    glfwDestroyWindow(window_);
}

void Window::pollEvents() const {
    glfwPollEvents();
}

void Window::swapBuffers() const {
    glfwSwapBuffers(window_);
}

void Window::setupListeners() {
    glfwSetMouseButtonCallback(window_, &Window::mouseButtonHandler);
    glfwSetCursorPosCallback(window_, &Window::cursorMoveHandler);
    glfwSetCursorEnterCallback(window_, &Window::cursorEnterHandler);
    glfwSetScrollCallback(window_, &Window::scrollHandler);
    glfwSetKeyCallback(window_, &Window::keyHandler);
    std::cout << "Callback installed" << std::endl;
}

void Window::setListener(input::InputListener* inputListener) {
    inputListener_ = inputListener;
}

Window* Window::getWindow(GLFWwindow* window) {
    void* ptr = glfwGetWindowUserPointer(window);
    return static_cast<Window*>(ptr);
}

void Window::mouseButtonHandler(GLFWwindow* window, int button, int action,
        int mods) {
    if (Window* wnd = getWindow(window)) {
        wnd->mouseButtonHandler(button, action, mods);
    }
}

void Window::cursorMoveHandler(GLFWwindow* window, double x, double y) {
    if (Window* wnd = getWindow(window)) {
        wnd->cursorMoveHandler(x, y);
    }
}

void Window::cursorEnterHandler(GLFWwindow* window, int entered) {
    if (Window* wnd = getWindow(window)) {
        wnd->cursorEnterHandler(entered);
    }
}

void Window::scrollHandler(GLFWwindow* window, double dx, double dy) {
    if (Window* wnd = getWindow(window)) {
        wnd->scrollHandler(dx, dy);
    }
}

void Window::keyHandler(GLFWwindow* window, int key, int scancode, int action,
        int mods) {
    if (Window* wnd = getWindow(window)) {
        wnd->keyHandler(key, scancode, action, mods);
    }
}

void Window::mouseButtonHandler(int button, int action, int mods) {
    std::cout << "Mouse sth" << std::endl;
}

void Window::cursorMoveHandler(double x, double y) {
    std::cout << "Moving! (" << x << ", " << y << ")" << std::endl;
}

void Window::cursorEnterHandler(int entered) {
    std::cout << "Entered? " << entered << std::endl;
}

void Window::scrollHandler(double dx, double dy) {
    std::cout << "Scrolling: " << dx << ", " << dy << std::endl;
}

void Window::keyHandler(int key, int scancode, int action, int mods) {
    KeyEvent event { Key::UP, KeyEvent::Type::DOWN, Mod::SHIFT };

    if (inputListener_ != nullptr) {
        inputListener_->keyEvent(event);
    }
}

} /* namespace gfx */
} /* namespace zephyr */
