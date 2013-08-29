
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
#include <example/Speaker.hpp>

using namespace zephyr;


int main(int argc, char* argv[])
{
    Root root("resources/config.xml");
    core::Scheduler& sched = root.scheduler();
    core::TaskPtr task = std::make_shared<Speaker>("Dupa");
    sched.startTask("SpeakerA", 1, task);
    
    int width = root.config().get("zephyr.window.width", 800);
    int height = root.config().get("zephyr.window.height", 600);
    std::string title = root.config().get<std::string>("zephyr.window.title");
    std::cout << "Window size: " << width << "x" << height << std::endl;
    gfx::Window window(width, height, title);
    core::TaskPtr swapper = std::make_shared<gfx::BufferSwapper>(window);
    sched.startTask("swapper", 2, swapper);
    
    root.run();
    return 0;
}
