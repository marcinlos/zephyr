/**
 * @file Root.hpp
 */

#ifndef ZEPHYR_ROOT_HPP_
#define ZEPHYR_ROOT_HPP_

#include <zephyr/core/Scheduler.hpp>
#include <zephyr/core/Config.hpp>
#include <zephyr/core/MessageQueue.hpp>
#include <zephyr/core/DispatcherTask.hpp>
#include <zephyr/gfx/WindowSystem.hpp>
#include <zephyr/time/TimeSource.hpp>
#include <string>

namespace zephyr {

class Root {
public:

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


    /** Name of the dispatcher task, as registered in the schedulre */
    static constexpr char DISPATCHER_NAME[] = "msg-dispatcher-task";

    /**
     * Dispatcher should run relatively early, but some place may be needed
     * for core input subsystems, that I believe should run before the logic.
     */
    static const int DISPATCHER_PRIORITY = 1000;


    /** Name of the swapper task */
    static constexpr char SWAPPER_NAME[] = "buffer-swapper-task";

    /**
     * Probably swapper shold execute last, after all the other subsystems
     * finish their modifications and rendering.
     */
    static const int SWAPPER_PRIORITY = 1000000;


    /** Name of the task that polls the window events */
    static constexpr char WINDOW_POLLER_NAME[] = "window-poll-task";

    /** Should run before dispatcher */
    static const int WINDOW_POLLER_PRIORITY = 100;

private:
    core::Scheduler scheduler_;
    core::Config config_;

    core::MessageQueue messageQueue_;
    core::MessageDispatcher dispatcher_;

    time::TimeSource timeSource_;

    std::unique_ptr<gfx::WindowSystem> window_;

    /**
     * Initializes all the immediately necessary subsystems.
     * */
    void setup();

    /**
     * Creates and runs crucial tasks:
     */
    void runCoreTasks();

};

} /* namespace zephyr */

#endif /* ZEPHYR_ROOT_HPP_ */
