/*
 * WindowSystem.cpp
 *
 *  Created on: Aug 31, 2013
 *      Author: los
 */

#include <zephyr/window/WindowSystem.hpp>
#include <zephyr/window/BufferSwapper.hpp>
#include <zephyr/window/EventPoller.hpp>
#include <zephyr/util/make_unique.hpp>
#include <iostream>

namespace zephyr {
namespace window {


WindowSystem::WindowSystem(Context ctx)
: config_(ctx.config)
{
    std::cout << "[Window] Initializing the subsystem" << std::endl;
    window_ = createWindow();
    runTasks(ctx.scheduler);
    std::cout << "[Window] Subsystem initialized" << std::endl;
}



std::unique_ptr<Window> WindowSystem::createWindow() {
    int width = config_.get("zephyr.window.width", 800);
    int height = config_.get("zephyr.window.height", 600);
    std::string title = config_.get<std::string>("zephyr.window.title");

    std::cout << "[Window] Size: " << width << "x" << height << std::endl;
    return util::make_unique<Window>(width, height, title);
}

void WindowSystem::runTasks(core::Scheduler& scheduler) {
    std::cout << "[Window] Registering swapper - " << SWAPPER_PRIORITY << std::endl;
    core::TaskPtr swapper = std::make_shared<BufferSwapper>(*window_);
    scheduler.startTask(SWAPPER_NAME, SWAPPER_PRIORITY, swapper);

    std::cout << "[Window] Registering poller - " << WINDOW_POLLER_PRIORITY << std::endl;
    core::TaskPtr poller = std::make_shared<EventPoller>(*window_);
    scheduler.startTask(WINDOW_POLLER_NAME, WINDOW_POLLER_PRIORITY, poller);
}


/// Static member definitions
constexpr char WindowSystem::SWAPPER_NAME[];
constexpr char WindowSystem::WINDOW_POLLER_NAME[];

} /* namespace window */
} /* namespace zephyr */
