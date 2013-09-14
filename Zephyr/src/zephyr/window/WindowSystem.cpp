/*
 * WindowSystem.cpp
 *
 *  Created on: Aug 31, 2013
 *      Author: los
 */

#include <zephyr/window/WindowSystem.hpp>
#include <zephyr/window/BufferSwapper.hpp>
#include <zephyr/window/EventPoller.hpp>
#include <zephyr/window/messages.hpp>
#include <zephyr/input/messages.hpp>
#include <zephyr/input/MessageGenerator.hpp>
#include <zephyr/util/make_unique.hpp>
#include <iostream>

namespace zephyr {
namespace window {


WindowSystem::WindowSystem(Context ctx)
: config_(ctx.config)
{
    std::cout << "[Window] Initializing the subsystem" << std::endl;
    window_ = createWindow(ctx);
    attachInputListener(ctx);
    runTasks(ctx.scheduler);

    registerHandler(ctx.dispatcher, msg::WINDOW, this, &WindowSystem::receive);

    registerHandler(ctx.dispatcher, input::msg::INPUT_SOURCE, this,
            &WindowSystem::receiveAsInputSource);

    std::cout << "[Window] Subsystem initialized" << std::endl;
}

std::unique_ptr<Window> WindowSystem::createWindow(const Context& ctx) {
    int width = config_.get("zephyr.window.width", 800);
    int height = config_.get("zephyr.window.height", 600);
    std::string title = config_.get<std::string>("zephyr.window.title");
    bool fullscreen = config_.get<bool>("zephyr.window.fullscreen", false);
    bool capture = config_.get<bool>("zephyr.window.capture-mouse", false);

    std::cout << "[Window] Size: " << width << "x" << height << std::endl;
    return util::make_unique<Window>(InitInfo {
        width,
        height,
        title,
        fullscreen,
        capture ? MouseMode::RELATIVE : MouseMode::ABSOLUTE
    }, ctx.messageQueue);
}

void WindowSystem::runTasks(core::Scheduler& scheduler) {
    std::cout << "[Window] Registering swapper - " << SWAPPER_PRIORITY << std::endl;
    core::TaskPtr swapper = std::make_shared<BufferSwapper>(*window_);
    scheduler.startTask(SWAPPER_NAME, SWAPPER_PRIORITY, swapper);

    std::cout << "[Window] Registering poller - " << WINDOW_POLLER_PRIORITY << std::endl;
    core::TaskPtr poller = std::make_shared<EventPoller>(*window_);
    scheduler.startTask(WINDOW_POLLER_NAME, WINDOW_POLLER_PRIORITY, poller);
}

void WindowSystem::attachInputListener(const Context& ctx) {
    using input::MessageGenerator;

    std::cout << "[Window] Creating input listener" << std::endl;
    ListenerPtr listener = std::make_shared<MessageGenerator>(ctx.messageQueue);
    window_->setListener(listener);
}

void WindowSystem::receive(const Message& message) {
    std::cout << "[Window system] " << message << std::endl;
    switch (message.type) {
    case msg::FULLSCREEN_ON:
        window_->fullscreen(true);
        break;

    case msg::FULLSCREEN_OFF:
        window_->fullscreen(false);
        break;

    case msg::TOGGLE_FULLSCREEN:
        window_->toggleFullscreen();
        break;
    }
}

void WindowSystem::receiveAsInputSource(const Message& message) {
    std::cout << "[Window system (as input source)] " << message << std::endl;
    switch (message.type) {
    case input::msg::MOUSE_ABS:
        window_->mouseMode(MouseMode::ABSOLUTE);
        break;

    case input::msg::MOUSE_REL:
        window_->mouseMode(MouseMode::RELATIVE);
        break;

    case input::msg::MOUSE_TOGGLE:
        window_->toggleMouseMode();
        break;
    }
}

/// Static member definitions
constexpr char WindowSystem::SWAPPER_NAME[];
constexpr char WindowSystem::WINDOW_POLLER_NAME[];

} /* namespace window */
} /* namespace zephyr */
