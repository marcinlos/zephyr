#include <iostream>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include "core/Scheduler.hpp"

using namespace glm;
using namespace zephyr;

int main(int argc, char* argv[])
{
    core::Scheduler scheduler;

    scheduler.run();
    return 0;
}
