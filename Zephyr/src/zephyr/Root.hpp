/**
 * @file Root.hpp
 */

#ifndef ZEPHYR_ROOT_HPP_
#define ZEPHYR_ROOT_HPP_

#include <zephyr/core/Scheduler.hpp>
#include <zephyr/core/Config.hpp>
#include <zephyr/core/MessageQueue.hpp>
#include <zephyr/core/DispatcherTask.hpp>
#include <zephyr/gfx/Window.hpp>
#include <string>

namespace zephyr {

class Root {
public:

    /** Name of the dispatcher task, as registered in the schedulre */
    static constexpr char DISPATCHER_NAME[] = "dispatcher-task";

    static const int DISPATCHER_PRIORITY = 100;

    /** Initializes the engine using data from the XML configuration file */
    Root(const std::string& configStream);

    /** Initializes the engine using data from the XML configuration stream */
    Root(std::istream& config);

    /** Runs the main loop */
    void run();

    /**
     * @return Task scheduler
     */
    core::Scheduler& scheduler() {
        return scheduler_;
    }

    /**
     * @return Configuration
     */
    core::Config& config() {
        return config_;
    }

    /**
     * @return Message dispatcher
     */
    core::MessageDispatcher& dispatcher() {
        return dispatcher_;
    }

private:
    core::Scheduler scheduler_;
    core::Config config_;

    core::MessageQueue messageQueue_;
    core::MessageDispatcher dispatcher_;

    std::unique_ptr<gfx::Window> window_;
public:
    void setup();

};

} /* namespace zephyr */

#endif /* ZEPHYR_ROOT_HPP_ */
