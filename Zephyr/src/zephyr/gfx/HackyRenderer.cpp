/**
 * @file HackyRenderer.cpp
 */

#include <zephyr/gfx/HackyRenderer.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <boost/io/ios_state.hpp>

namespace zephyr {
namespace gfx {

typedef boost::io::ios_all_saver guard;

HackyRenderer::HackyRenderer(Context ctx)
: clocks(ctx.clockManager)
{
    std::cout << "[Hacky] Initializing hacky renderer" << std::endl;
    prevTime = clock.time();
    counter = 0;
    glfwSwapInterval(0);
}

void HackyRenderer::updateTime() {
    ++counter;
    double time = clock.time();
    if (time - prevTime > 1.0) {
        double fps = counter / (time - prevTime);
        {
            guard g(std::cout);
            std::cout.precision(1);
            std::cout.setf(std::ios::fixed);
            std::cout << "FPS: " << fps << std::endl;
        }
        counter = 0;
        prevTime = time;
    }
}

void HackyRenderer::update() {
    updateTime();

    int w, h;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
    glViewport(0, 0, w, h);

    glClearColor(0.4f, 0.6f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    float ratio;
    ratio = w / (float) h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(clock.time() * 100.f, 0.f, 0.f, 1.f);
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-0.6f, -0.4f, 0.f);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.6f, -0.4f, 0.f);
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.6f, 0.f);
    glEnd();
}


} /* namespace gfx */
} /* namespace zephyr */

