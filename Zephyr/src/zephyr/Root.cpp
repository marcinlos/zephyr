#include <zephyr/Root.hpp>
#include <iostream>

namespace zephyr {

Root::Root(const std::string& config_path) {
    std::cout << "[Root] Reading configuration from \""
            << config_path << "\"" << std::endl;
    config_.loadXML(config_path);
}

Root::Root(std::istream& config_stream) {
    config_.loadXML(config_stream);
}

void Root::run() {
    scheduler_.run();
}

} /* namespace zephyr */
