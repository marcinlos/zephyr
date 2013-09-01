
#include <zephyr/Root.hpp>
#include <zephyr/core/DispatcherTask.hpp>
#include <zephyr/time/TimeSource.hpp>
#include <zephyr/util/make_unique.hpp>
#include <iostream>
#include <memory>


#include <zephyr/gfx/HackyRenderer.hpp>

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
    runCoreTasks();

    Context ctx {
        config_,
        messageQueue_,
        dispatcher_,
        scheduler_,
        clockManager_
    };
    window_ = util::make_unique<window::WindowSystem>(ctx);
    input_ = util::make_unique<input::InputSystem>(ctx);

    TaskPtr task = std::make_shared<gfx::HackyRenderer>(ctx);
    scheduler_.startTask("hacky-renderer", 500000, task);

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

} /* namespace zephyr */
