/**
 * @file HackyRenderer.cpp
 */

#include <zephyr/gfx/HackyRenderer.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

namespace zephyr {
namespace gfx {


HackyRenderer::HackyRenderer(Context ctx)
: clocks(ctx.clockManager)
{
    std::cout << "[Hacky] Initializing hacky renderer" << std::endl;
    prevTime = clock.time();
    counter = 0;
}

void HackyRenderer::updateTime() {
    ++counter;
    double time = clock.time();
    if (time - prevTime > 1.0) {
        std::cout << "FPS: " << counter / (time - prevTime) << std::endl;
        counter = 0;
        prevTime = time;
    }
}

void HackyRenderer::update() {
    updateTime();

    glClearColor(0.4f, 0.8f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}


} /* namespace gfx */
} /* namespace zephyr */

