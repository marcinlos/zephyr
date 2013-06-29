#include <iostream>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include <memory>

#include <core/Scheduler.hpp>
#include <example/Speaker.hpp>

using namespace glm;
using namespace zephyr;

int main(int argc, char* argv[])
{
    core::Scheduler scheduler;
    scheduler.startTask("SpeakerA", 1, std::make_shared<Speaker>("Dupa"));
    scheduler.run();
    return 0;
}
