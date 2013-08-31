
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <memory>
#include <string>

#include <zephyr/Root.hpp>
#include <zephyr/gfx/Window.hpp>
#include <zephyr/gfx/BufferSwapper.hpp>
#include <zephyr/gfx/EventPoller.hpp>
#include <example/Speaker.hpp>

#include <zephyr/input/KeyEvent.hpp>

using namespace zephyr;

int main(int argc, char* argv[]) {

    Root root("resources/config.xml");
    core::Scheduler& sched = root.scheduler();

    int width = root.config().get("zephyr.window.width", 800);
    int height = root.config().get("zephyr.window.height", 600);
    std::string title = root.config().get<std::string>("zephyr.window.title");

    std::cout << "Window size: " << width << "x" << height << std::endl;
    gfx::Window window(width, height, title);

    core::TaskPtr swapper = std::make_shared<gfx::BufferSwapper>(window);
    sched.startTask("swapper", 10000, swapper);

    core::TaskPtr poller = std::make_shared<gfx::EventPoller>(window);
    sched.startTask("wnd-event-poller", 10, poller);

    root.run();
    return 0;
}
