/**
 * @file Window.cpp
 */

#include <zephyr/window/Window.hpp>
#include <zephyr/glfw/input_adapter.hpp>
#include <stdexcept>
#include <iostream>

namespace zephyr {
namespace window {

using input::Position;
using input::Button;
using input::Key;
using input::Mod;
using input::KeyEvent;
using input::ButtonEvent;


Window::Window(int width, int height, const std::string& title)
: inputListener_(nullptr)
{
    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_) {
        throw std::runtime_error("Window initialization failure");
    }
    glfwSetWindowUserPointer(window_, this);
    glfwMakeContextCurrent(window_);
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
    glfwSetMouseButtonCallback(window_, &Window::mouseHandler);
    glfwSetCursorPosCallback(window_, &Window::cursorHandler);
    glfwSetCursorEnterCallback(window_, &Window::focusHandler);
    glfwSetScrollCallback(window_, &Window::scrollHandler);
    glfwSetKeyCallback(window_, &Window::keyHandler);
    glfwSetWindowCloseCallback(window_, &Window::closeHandler);

    std::cout << "Callback installed" << std::endl;
}

void Window::setListener(const ListenerPtr& inputListener) {
    inputListener_ = inputListener;
}


input::Position Window::getCursorPosition() {
    double x, y;
    glfwGetCursorPos(window_, &x, &y);
    return {x, y};
}


void Window::mouseHandler(int button, int action, int mods) {
    Position pos = getCursorPosition();
    ButtonEvent event = glfw::buttonEventFromGLFW(button, action, mods, pos);

    std::cout << event << std::endl;

    if (inputListener_) {
        inputListener_->buttonEvent(event);
    }
}

void Window::cursorHandler(double x, double y) {
    Position pos {x, y};
    std::cout << "MoveEvent{pos=" << pos << "}" << std::endl;

    if (inputListener_) {
        inputListener_->mouseMove(pos);
    }
}

void Window::focusHandler(int entered) {
    std::cout << "Entered? " << entered << std::endl;
}

void Window::scrollHandler(double dx, double dy) {
    std::cout << "Scrolling: " << dx << ", " << dy << std::endl;

    if (inputListener_) {
        inputListener_->scroll(dy);
    }
}

void Window::keyHandler(int key, int scancode, int action, int mods) {
    // Convert to more friendly format
    KeyEvent event = glfw::keyEventFromGLFW(key, action, mods);

    std::cout << event << std::endl;

    if (inputListener_) {
        inputListener_->keyEvent(event);
    }
}

void Window::closeHandler() {
    std::cout << "Closing" << std::endl;
}


Window* Window::getWindow(GLFWwindow* window) {
    void* ptr = glfwGetWindowUserPointer(window);

    // Fail-fast behaviour, if the pointer is null something went really wrong
    if (ptr) {
        return static_cast<Window*>(ptr);
    } else {
        throw std::runtime_error("No pointer associated with window!");
    }
}

void Window::mouseHandler(GLFWwindow* window, int button, int action, int mods) {
    getWindow(window)->mouseHandler(button, action, mods);
}

void Window::cursorHandler(GLFWwindow* window, double x, double y) {
    getWindow(window)->cursorHandler(x, y);
}

void Window::focusHandler(GLFWwindow* window, int entered) {
    getWindow(window)->focusHandler(entered);
}

void Window::scrollHandler(GLFWwindow* window, double dx, double dy) {
    getWindow(window)->scrollHandler(dx, dy);
}

void Window::keyHandler(GLFWwindow* window, int key, int scancode, int action,
        int mods) {
    getWindow(window)->keyHandler(key, scancode, action, mods);
}

void Window::closeHandler(GLFWwindow* window) {
    getWindow(window)->closeHandler();
}


} /* namespace window */
} /* namespace zephyr */
