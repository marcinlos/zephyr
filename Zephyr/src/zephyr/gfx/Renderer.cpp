/**
 * @file Renderer.cpp
 */

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <zephyr/gfx/Renderer.hpp>


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

void Renderer::drawMesh(const MeshPtr& mesh) {
    glBindVertexArray(mesh->glName);
    GLenum mode = primitiveToGL(mesh->mode);
    if (mesh->indexed) {
        glDrawElements(mode, mesh->count, mesh->indexType, 0);
    } else {
        glDrawArrays(mode, 0, mesh->count);
    }
    glBindVertexArray(0);
}

void Renderer::setMaterial(const MaterialPtr& material) {
    if (currentProgram_ != material->program) {
        glUseProgram(material->program->ref());
        currentProgram_ = material->program;
    }
    for (const auto& needed : currentProgram_->uniforms()) {
        const std::string& name = needed.first;
        if (Uniform* value = uniforms_.uniform(name)) {
            value->set(needed.second);
        }
    }
}

void Renderer::setModelTransform(const glm::mat4& transform) {
    GLint location = currentProgram_->uniformLocation("modelMatrix");
    auto data = glm::value_ptr(transform);
    glUniformMatrix4fv(location, 1, GL_FALSE, data);
}

void Renderer::render() {
    updateViewport();
    clearBuffers();

    for (const Renderable& item : renderables_) {
        setMaterial(item.entity->material);
        setModelTransform(item.transform);
        drawMesh(item.entity->mesh);
    }
    renderables_.clear();
}



} /* namespace gfx */
} /* namespace zephyr */

