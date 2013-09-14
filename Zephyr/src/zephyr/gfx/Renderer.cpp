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

void Renderer::render() {
    updateViewport();
    clearBuffers();

    for (auto&& item : renderables_) {
        MaterialPtr material = item.entity->material;
        glUseProgram(material->program->ref());
        GLint location = material->program->uniformLocation("modelMatrix");
        auto data = glm::value_ptr(item.transform);
        glUniformMatrix4fv(location, 1, GL_FALSE, data);

        const MeshPtr& mesh = item.entity->mesh;
        glBindVertexArray(mesh->glName);
        GLenum mode = primitiveToGL(mesh->mode);
        if (mesh->indexed) {
            glDrawElements(mode, mesh->count, mesh->indexType, 0);
        } else {
            glDrawArrays(mode, 0, mesh->count);
        }
        glBindVertexArray(0);
    }
    renderables_.clear();
}



} /* namespace gfx */
} /* namespace zephyr */

