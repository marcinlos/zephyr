/**
 * @file Window.cpp
 */

#include <zephyr/window/Window.hpp>
#include <zephyr/glfw/input_adapter.hpp>
#include <zephyr/messages.hpp>
#include <stdexcept>
#include <iostream>

namespace zephyr {
namespace window {

using input::Button;
using input::Key;
using input::Mod;
using input::KeyEvent;
using input::ButtonEvent;


Window::Window(const InitInfo& info, MessageQueue& queue)
: queue(queue)
, inputListener_(nullptr)
, fullscreen_(info.fullscreen)
, mouseMode_(info.mode)
, title_(info.title)
{
    window_ = createWindow(info);
    if (fullscreen_) {
        enableFullscren();
    }
    glfwMakeContextCurrent(window_.get());
    mouseMode(mouseMode_);
}

void Window::pollEvents() const {
    glfwPollEvents();
}

void Window::swapBuffers() const {
    glfwSwapBuffers(window_.get());
}

void Window::setupListeners(GLFWwindow* window) const {
    glfwSetMouseButtonCallback(window, &Window::mouseHandler);
    glfwSetCursorPosCallback(window, &Window::cursorHandler);
    glfwSetCursorEnterCallback(window, &Window::focusHandler);
    glfwSetScrollCallback(window, &Window::scrollHandler);
    glfwSetKeyCallback(window, &Window::keyHandler);
    glfwSetWindowCloseCallback(window, &Window::closeHandler);

    std::cout << "Callback installed" << std::endl;
}

void Window::setListener(const ListenerPtr& inputListener) {
    inputListener_ = inputListener;
}

Window::WindowPtr Window::createWindow(const InitInfo& info) const {
    int width = info.width;
    int height = info.height;
    const char* title = info.title.c_str();
    GLFWmonitor* monitor = info.fullscreen ? glfwGetPrimaryMonitor() : nullptr;

    WindowPtr wnd { glfwCreateWindow(width, height, title, monitor, nullptr) };
    if (!wnd) {
        throw std::runtime_error("Window initialization failure");
    }
    setupListeners(wnd.get());
    glfwSetWindowUserPointer(wnd.get(), const_cast<Window*>(this));
    return wnd;
}


void Window::fullscreen(bool full) {
    if (full) {
        enableFullscren();
    } else {
        disableFullscreen();
    }
    fullscreen_ = full;
    window_ = createWindow({
        state_.width,
        state_.height,
        title_,
        full
    });
    glfwMakeContextCurrent(window_.get());
}

bool Window::toggleFullscreen() {
    fullscreen(!fullscreen_);
    return !fullscreen_;
}


void Window::enableFullscren() {
    glfwGetWindowSize(window_.get(), &state_.width, &state_.height);
}

void Window::disableFullscreen() {
    // empty
}

inline int mouseModeToGLFW(MouseMode mode) {
    switch (mode) {
    case MouseMode::ABSOLUTE: return GLFW_CURSOR_NORMAL;
    case MouseMode::RELATIVE: return GLFW_CURSOR_DISABLED;
    default:
        throw std::runtime_error("Invalid input mode");
    }
}

inline MouseMode nextMouseMode(MouseMode mode) {
    int ord = static_cast<std::uint32_t>(mode);
    int total = static_cast<std::uint32_t>(MouseMode::LAST) + 1;
    return static_cast<MouseMode>((ord + 1) % total);
}

void Window::mouseMode(MouseMode mode) {
    mouseMode_ = mode;
    int value = mouseModeToGLFW(mode);
    glfwSetInputMode(window_.get(), GLFW_CURSOR, value);
}

void Window::toggleMouseMode() {
    mouseMode_ = nextMouseMode(mouseMode_);
    mouseMode(mouseMode_);
}


Position Window::getCursorPosition() const {
    double x, y;
    glfwGetCursorPos(window_.get(), &x, &y);
    return {x, y};
}


void Window::mouseHandler(int button, int action, int mods) {
    Position pos = getCursorPosition();
    ButtonEvent event = glfw::buttonEventFromGLFW(button, action, mods, pos);

    if (inputListener_) {
        inputListener_->buttonEvent(event);
    }
}

void Window::cursorHandler(double x, double y) {
    Position pos {x, y};

    if (inputListener_) {
        inputListener_->mouseMove(pos);
    }
}

void Window::focusHandler(int entered) {
    std::cout << "Entered? " << entered << std::endl;
}

void Window::scrollHandler(double dx, double dy) {
    if (inputListener_) {
        inputListener_->scroll(dy);
    }
}

void Window::keyHandler(int key, int scancode, int action, int mods) {
    // Convert to more friendly format
    KeyEvent event = glfw::keyEventFromGLFW(key, action, mods);

    if (inputListener_) {
        inputListener_->keyEvent(event);
    }
}

void Window::closeHandler() {
    queue.post({
        zephyr::msg::SYSTEM,
        zephyr::msg::QUIT
    });
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
