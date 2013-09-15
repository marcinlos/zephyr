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
#include <zephyr/time/ClockManager.hpp>
#include <zephyr/util/static_hash.hpp>
#include <zephyr/gfx/GraphicsSystem.hpp>
#include <zephyr/core/Register.hpp>
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

    /** @addtogroup SystemTasks */
    /// @{

    /**
     * Name of the dispatcher task
     */
    static constexpr char DISPATCHER_NAME[] = "msg-dispatcher-task";

    /**
     * Dispatcher should run relatively early, but some place may be needed
     * for core input subsystems, that I believe should run before the logic.
     */
    static const int DISPATCHER_PRIORITY = 1000;

    /**
     * Name of the clock updater task
     */
    static constexpr char CLOCK_UPDATER_NAME[] = "clock-updater-task";

    /**
     * Clock should be updated early, before any noncore activity takes place.
     */
    static const int CLOCK_UPDATER_PRIORITY = 110;

    /// @}

    core::Scheduler& scheduler() {
        return scheduler_;
    }

    core::Config& config() {
        return config_;
    }

    core::Register& vars() {
        return register_;
    }

    core::MessageQueue& messageQueue() {
        return messageQueue_;
    }

    core::MessageDispatcher& dispatcher() {
        return dispatcher_;
    }

    time::ClockManager& clockManager() {
        return clockManager_;
    }

    window::WindowSystem& window() {
        return *window_;
    }

    input::InputSystem& input() {
        return *input_;
    }

    gfx::GraphicsSystem& graphics() {
        return *graphics_;
    }

private:
    core::Scheduler scheduler_;
    core::Config config_;

    core::Register register_;

    core::MessageQueue messageQueue_;
    core::MessageDispatcher dispatcher_;

    time::ClockManager clockManager_;

    std::unique_ptr<window::WindowSystem> window_;

    std::unique_ptr<input::InputSystem> input_;

    std::unique_ptr<gfx::GraphicsSystem> graphics_;

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
