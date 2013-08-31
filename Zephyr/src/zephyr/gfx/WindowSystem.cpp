/*
 * WindowSystem.cpp
 *
 *  Created on: Aug 31, 2013
 *      Author: los
 */

#include <zephyr/gfx/WindowSystem.hpp>
#include <zephyr/gfx/BufferSwapper.hpp>
#include <zephyr/gfx/EventPoller.hpp>
#include <zephyr/util/make_unique.hpp>
#include <iostream>

namespace zephyr {
namespace gfx {


WindowSystem::WindowSystem(Context ctx)
: config_(ctx.config)
{
    std::cout << "Initializing the window subsystem" << std::endl;
    window_ = createWindow();
    runTasks(ctx.scheduler);
    std::cout << "Window subsystem initialized" << std::endl;
}



std::unique_ptr<gfx::Window> WindowSystem::createWindow() {
    int width = config_.get("zephyr.window.width", 800);
    int height = config_.get("zephyr.window.height", 600);
    std::string title = config_.get<std::string>("zephyr.window.title");

    std::cout << "Window size: " << width << "x" << height << std::endl;
    return util::make_unique<gfx::Window>(width, height, title);
}

void WindowSystem::runTasks(core::Scheduler& scheduler) {
    core::TaskPtr swapper = std::make_shared<gfx::BufferSwapper>(*window_);
    scheduler.startTask(SWAPPER_NAME, SWAPPER_PRIORITY, swapper);

    core::TaskPtr poller = std::make_shared<gfx::EventPoller>(*window_);
    scheduler.startTask(WINDOW_POLLER_NAME, WINDOW_POLLER_PRIORITY, poller);
}


/// Static member definitions
constexpr char WindowSystem::SWAPPER_NAME[];
constexpr char WindowSystem::WINDOW_POLLER_NAME[];

} /* namespace gfx */
} /* namespace zephyr */
