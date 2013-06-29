#include <iostream>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include <memory>

#include <zephyr/Root.hpp>
#include <example/Speaker.hpp>

using namespace glm;
using namespace zephyr;

int main(int argc, char* argv[])
{
    Root root;
    core::Scheduler& sched = root.scheduler;
    core::TaskPtr task = std::make_shared<Speaker>("Dupa");
    sched.startTask("SpeakerA", 1, task);
    root.run();
    return 0;
}
