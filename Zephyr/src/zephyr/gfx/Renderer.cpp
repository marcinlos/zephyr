/**
 * @file Renderer.cpp
 */

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <zephyr/gfx/Renderer.hpp>

#include <zephyr/gfx/uniforms.hpp>


namespace zephyr {
namespace gfx {


Renderer::Renderer() {
    glewInit();
    glfwSwapInterval(vsync_);

    setCulling();
    setDepthTest();

    updateViewport();
}

void Renderer::updateViewport() {
    int w, h;
    GLFWwindow* window = glfwGetCurrentContext();
    glfwGetFramebufferSize(window, &w, &h);
    viewport_.set(0, 0, w, h);
    glViewport(0, 0, w, h);
}

void Renderer::setCulling() {
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
}

void Renderer::setDepthTest() {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glDepthRange(0.0f, 1.0f);
}

void Renderer::toggleVSync() {
    glfwSwapInterval(vsync_ = !vsync_);
}

void Renderer::clearBuffers() {
    glClearColor(0.6f, 0.6f, 0.8f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::update() {
    updateViewport();
    clearBuffers();

}



} /* namespace gfx */
} /* namespace zephyr */

