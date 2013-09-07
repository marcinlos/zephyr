/**
 * @file Root.cpp
 */

#include <zephyr/Root.hpp>
#include <zephyr/messages.hpp>
#include <zephyr/core/DispatcherTask.hpp>
#include <zephyr/time/ClockUpdateTask.hpp>
#include <zephyr/util/make_unique.hpp>
#include <iostream>
#include <memory>


#include <zephyr/gfx/HackyRenderer.hpp>

namespace zephyr {

using namespace core;


Root::Root(const std::string& configPath) {
    std::cout << "[Root] Reading configuration from \"" << configPath
            << "\"" << std::endl;

    config.loadXML(configPath);
    setup();
}

Root::Root(std::istream& configStream) {
    config.loadXML(configStream);
    setup();
}

void Root::setup() {
    std::cout << "[Root] Creating dispatch task" << std::endl;

    runCoreTasks();
    initSubsystems();

    registerHandler(dispatcher, msg::SYSTEM, this, &Root::receive);

    std::cout << "[Root] Initialization completed" << std::endl;
}

void Root::initSubsystems() {
    Context ctx {
        config,
        messageQueue,
        dispatcher,
        scheduler,
        clockManager
    };
    window = util::make_unique<window::WindowSystem>(ctx);
    input = util::make_unique<input::InputSystem>(ctx);


    TaskPtr task = std::make_shared<gfx::HackyRenderer>(ctx);
    scheduler.startTask("hacky-renderer", 500000, task);
}

void Root::runCoreTasks() {
    TaskPtr task = std::make_shared<DispatcherTask>(messageQueue, dispatcher);
    scheduler.startTask(DISPATCHER_NAME, DISPATCHER_PRIORITY, task);

    TaskPtr clockTask = std::make_shared<time::ClockUpdateTask>(clockManager);
    scheduler.startTask(CLOCK_UPDATER_NAME, CLOCK_UPDATER_PRIORITY, clockTask);
}

void Root::run() {
    std::cout << "[Root] Running..." << std::endl;
    scheduler.run();
    std::cout << "[Root] Stopped" << std::endl;
}

void Root::receive(const core::Message& message) {
    std::cout << "[Root] " << message << std::endl;
    if (message.type == msg::QUIT) {
        std::cout << "[Root] Quitting..." << std::endl;
        scheduler.stop();
    }
}

// Static member definitions

constexpr char Root::DISPATCHER_NAME[];

constexpr char Root::CLOCK_UPDATER_NAME[];

} /* namespace zephyr */

