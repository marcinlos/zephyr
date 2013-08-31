#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <zephyr/Root.hpp>
#include <zephyr/core/DispatcherTask.hpp>
#include <zephyr/gfx/Window.hpp>
#include <zephyr/gfx/BufferSwapper.hpp>
#include <zephyr/gfx/EventPoller.hpp>

#include <iostream>
#include <memory>


namespace zephyr {

using namespace core;


Root::Root(const std::string& configPath) {
    std::cout << "[Root] Reading configuration from \"" << configPath
            << "\"" << std::endl;

    config_.loadXML(configPath);
    setup();
}

Root::Root(std::istream& configStream) {
    config_.loadXML(configStream);
    setup();
}

void Root::setup() {
    std::cout << "[Root] Creating dispatch task" << std::endl;

    TaskPtr task = std::make_shared<DispatcherTask>(messageQueue_, dispatcher_);
    scheduler_.startTask(DISPATCHER_NAME, DISPATCHER_PRIORITY, task);

    int width = config_.get("zephyr.window.width", 800);
    int height = config_.get("zephyr.window.height", 600);
    std::string title = config_.get<std::string>("zephyr.window.title");

    std::cout << "Window size: " << width << "x" << height << std::endl;
    window_.reset(new gfx::Window(width, height, title));

    core::TaskPtr swapper = std::make_shared<gfx::BufferSwapper>(*window_);
    scheduler_.startTask("swapper", 10000, swapper);

    core::TaskPtr poller = std::make_shared<gfx::EventPoller>(*window_);
    scheduler_.startTask("wnd-event-poller", 10, poller);

    std::cout << "[Root] Initialization completed" << std::endl;
}

void Root::run() {
    std::cout << "[Root] Running..." << std::endl;
    scheduler_.run();
    std::cout << "[Root] Shutting down..." << std::endl;
}

// Static member definition
constexpr char Root::DISPATCHER_NAME[];

} /* namespace zephyr */
