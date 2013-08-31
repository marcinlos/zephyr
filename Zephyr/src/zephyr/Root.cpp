
#include <zephyr/Root.hpp>
#include <zephyr/core/DispatcherTask.hpp>
#include <zephyr/gfx/Window.hpp>
#include <zephyr/gfx/BufferSwapper.hpp>
#include <zephyr/gfx/EventPoller.hpp>

#include <zephyr/time/TimeSource.hpp>

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
    Context ctx {config_, messageQueue_, dispatcher_, scheduler_};
    runCoreTasks();
    window_ = util::make_unique<gfx::WindowSystem>(ctx);

    std::cout << "[Root] Initialization completed" << std::endl;
}

void Root::runCoreTasks() {
    TaskPtr task = std::make_shared<DispatcherTask>(messageQueue_, dispatcher_);
    scheduler_.startTask(DISPATCHER_NAME, DISPATCHER_PRIORITY, task);
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
