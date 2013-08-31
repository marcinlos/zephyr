
#include <zephyr/Root.hpp>
#include <zephyr/core/DispatcherTask.hpp>
#include <iostream>


namespace zephyr {

using namespace core;


Root::Root(const std::string& configPath) {
    std::cout << "[Root] Reading configuration from \"" << configPath
            << "\"" << std::endl;

    config_.loadXML(configPath);
    setupDispatchTask();
}

Root::Root(std::istream& configStream) {
    config_.loadXML(configStream);
    setupDispatchTask();
}

void Root::setupDispatchTask() {
    std::cout << "[Root] Creating dispatch task" << std::endl;

    TaskPtr task = std::make_shared<DispatcherTask>(messageQueue_, dispatcher_);
    scheduler_.startTask(DISPATCHER_NAME, DISPATCH_PRIORITY, task);
}

void Root::run() {
    scheduler_.run();
}

// Static member definition
constexpr char Root::DISPATCHER_NAME[];

} /* namespace zephyr */
