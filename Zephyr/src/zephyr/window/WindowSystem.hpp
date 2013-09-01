/**
 * @file WindowSystem.hpp
 */

#ifndef ZEPHYR_GFX_WINDOWSYSTEM_HPP_
#define ZEPHYR_GFX_WINDOWSYSTEM_HPP_

#include <zephyr/window/Window.hpp>
#include <zephyr/Context.hpp>
#include <memory>


namespace zephyr {
namespace window {

/**
 * Handles details of window creation and integration with the rest of the
 * engine.
 */
class WindowSystem {
public:

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


    WindowSystem(Context ctx);

private:
    core::Config& config_;

    std::unique_ptr<Window> window_;

    /**
     * Creates window subsystem.
     */
    std::unique_ptr<Window> createWindow();

    /**
     * Runs window-specific tasks.
     */
    void runTasks(core::Scheduler& scheduler);

    /**
     * Creates input listener and attaches it to the window.
     */
    void attachInputListener(const Context& ctx);

};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_WINDOWSYSTEM_HPP_ */
