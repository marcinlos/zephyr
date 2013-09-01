/**
 * @file Root.hpp
 */

#ifndef ZEPHYR_ROOT_HPP_
#define ZEPHYR_ROOT_HPP_

#include <zephyr/core/Scheduler.hpp>
#include <zephyr/core/Config.hpp>
#include <zephyr/core/MessageQueue.hpp>
#include <zephyr/core/DispatcherTask.hpp>
#include <zephyr/window/WindowSystem.hpp>
#include <zephyr/input/InputSystem.hpp>
#include <zephyr/time/TimeSource.hpp>
#include <zephyr/util/static_hash.hpp>
#include <memory>
#include <string>

namespace zephyr {

/**
 * Main class of the framework. Reads configuration, performs the initialization
 * of core internal communication facilities and subsystems.
 */
class Root {
public:

    /** Initializes the engine using data from the XML configuration file */
    Root(const std::string& configStream);

    /** Initializes the engine using data from the XML configuration stream */
    Root(std::istream& config);

    /** Runs the main loop */
    void run();

    /** Name of the dispatcher task, as registered in the schedulre */
    static constexpr char DISPATCHER_NAME[] = "msg-dispatcher-task";

    /**
     * Dispatcher should run relatively early, but some place may be needed
     * for core input subsystems, that I believe should run before the logic.
     */
    static const int DISPATCHER_PRIORITY = 1000;

private:
    core::Scheduler scheduler;
    core::Config config;

    core::MessageQueue messageQueue;
    core::MessageDispatcher dispatcher;

    time::ClockManager clockManager;

    std::unique_ptr<window::WindowSystem> window;

    std::unique_ptr<input::InputSystem> input;

    /**
     * Initializes all the immediately necessary subsystems.
     */
    void setup();

    /**
     * Performs initialization on all the used subsystems
     */
    void initSubsystems();

    /**
     * Creates and runs crucial tasks:
     */
    void runCoreTasks();

    /**
     * Serves as the engine's root component message endpoint, for receiving
     * globally significant information from other components.
     */
    void receive(const core::Message& message);

};

} /* namespace zephyr */

#endif /* ZEPHYR_ROOT_HPP_ */
