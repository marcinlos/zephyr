
#include <zephyr/Root.hpp>
#include <zephyr/core/DispatcherTask.hpp>
#include <zephyr/gfx/Window.hpp>
#include <zephyr/gfx/BufferSwapper.hpp>
#include <zephyr/gfx/EventPoller.hpp>

#include <zephyr/util/make_unique.hpp>

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

    window_ = createWindow();
    runCoreTasks();

    std::cout << "[Root] Initialization completed" << std::endl;
}

std::unique_ptr<gfx::Window> Root::createWindow() {
    int width = config_.get("zephyr.window.width", 800);
    int height = config_.get("zephyr.window.height", 600);
    std::string title = config_.get<std::string>("zephyr.window.title");

    std::cout << "Window size: " << width << "x" << height << std::endl;
    return util::make_unique<gfx::Window>(width, height, title);
}

void Root::runCoreTasks() {
    TaskPtr task = std::make_shared<DispatcherTask>(messageQueue_, dispatcher_);
    scheduler_.startTask(DISPATCHER_NAME, DISPATCHER_PRIORITY, task);

    core::TaskPtr swapper = std::make_shared<gfx::BufferSwapper>(*window_);
    scheduler_.startTask(SWAPPER_NAME, SWAPPER_PRIORITY, swapper);

    core::TaskPtr poller = std::make_shared<gfx::EventPoller>(*window_);
    scheduler_.startTask(WINDOW_POLLER_NAME, WINDOW_POLLER_PRIORITY, poller);
}

void Root::run() {
    std::cout << "[Root] Running..." << std::endl;
    scheduler_.run();
    std::cout << "[Root] Shutting down..." << std::endl;
}

/// Static member definitions
constexpr char Root::DISPATCHER_NAME[];

constexpr char Root::SWAPPER_NAME[];

constexpr char Root::WINDOW_POLLER_NAME[];

} /* namespace zephyr */
