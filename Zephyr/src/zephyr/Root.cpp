/**
 * @file Root.cpp
 */

#include <zephyr/Root.hpp>
#include <zephyr/messages.hpp>
#include <zephyr/core/DispatcherTask.hpp>
#include <zephyr/time/ClockUpdateTask.hpp>
#include <zephyr/util/make_unique.hpp>
#include <zephyr/gfx/Renderer.hpp>
#include <iostream>
#include <memory>


#include <zephyr/gfx/HackyRenderer.hpp>

namespace zephyr {

using namespace core;
using resources::ResourceSystem;
using window::WindowSystem;
using input::InputSystem;
using gfx::GraphicsSystem;

using time::ClockUpdateTask;


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
    initSubsystems();

    registerHandler(dispatcher_, msg::SYSTEM, this, &Root::receive);

    std::cout << "[Root] Initialization completed" << std::endl;
}

void Root::initSubsystems() {
    Context ctx {
        config_,
        messageQueue_,
        dispatcher_,
        scheduler_,
        clockManager_
    };
    resources_ = util::make_unique<ResourceSystem>(config_);
    window_ = util::make_unique<WindowSystem>(ctx);
    input_ = util::make_unique<InputSystem>(ctx);
    graphics_ = util::make_unique<GraphicsSystem>(scheduler_, *resources_);
}

void Root::runCoreTasks() {
    TaskPtr task = std::make_shared<DispatcherTask>(messageQueue_, dispatcher_);
    scheduler_.startTask(DISPATCHER_NAME, DISPATCHER_PRIORITY, task);

    TaskPtr clockTask = std::make_shared<ClockUpdateTask>(clockManager_);
    scheduler_.startTask(CLOCK_UPDATER_NAME, CLOCK_UPDATER_PRIORITY, clockTask);
}

void Root::run() {
    std::cout << "[Root] Running..." << std::endl;
    scheduler_.run();
    std::cout << "[Root] Stopped" << std::endl;
}

void Root::receive(const core::Message& message) {
    std::cout << "[Root] " << message << std::endl;
    if (message.type == msg::QUIT) {
        std::cout << "[Root] Quitting..." << std::endl;
        scheduler_.stop();
    }
}

// Static member definitions

constexpr char Root::DISPATCHER_NAME[];

constexpr char Root::CLOCK_UPDATER_NAME[];

} /* namespace zephyr */

