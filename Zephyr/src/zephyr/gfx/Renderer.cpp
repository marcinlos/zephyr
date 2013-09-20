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
    glClearColor(0.3f, 0.5f, 0.9f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::drawMesh(const MeshPtr& mesh) {
    glBindVertexArray(mesh->id);
    GLenum mode = primitiveToGL(mesh->mode);
    if (mesh->indexed) {
        glDrawElements(mode, mesh->count, mesh->indexType, 0);
    } else {
        glDrawArrays(mode, 0, mesh->count);
    }
    glBindVertexArray(0);
}

inline GLenum textureType(TexDim dim) {
    switch (dim) {
    case TexDim::_1D: return GL_TEXTURE_1D;
    case TexDim::_2D: return GL_TEXTURE_2D;
    case TexDim::_3D: return GL_TEXTURE_3D;
    default: return -1;
    }
}

void Renderer::setMaterial(const MaterialPtr& material) {
    if (currentProgram_ != material->program) {
        glUseProgram(material->program->ref());
        currentProgram_ = material->program;
        if (!markAsLoaded(currentProgram_)) {
            for (const auto& blocks : currentProgram_->uniformBlocks()) {
                const std::string& name = blocks.first;
                GLuint index = blocks.second;
                GLuint bindingIndex = uniforms_.blockBindingIndex(name);
                currentProgram_->bindBlock(index, bindingIndex);
            }
        }
    }
    for (const auto& needed : currentProgram_->uniforms()) {
        const std::string& name = needed.first;
        if (Uniform* value = uniforms_.get(name)) {
            value->set(needed.second);
        }
    }
    for (const auto& local : material->uniforms) {
        const std::string& name = local.first;
        GLint slot = currentProgram_->uniformLocation(name);
        if (slot >= 0) {
            local.second->set(slot);
        }
    }
    int texUnit = 0;
    for (const auto& texPair : material->textures) {
        GLuint samplerUniform = texPair.first;
        const TexturePtr& texture = texPair.second;

        glUniform1i(samplerUniform, texUnit);

        glActiveTexture(GL_TEXTURE0 + texUnit);
        glBindTexture(textureType(texture->dim), texture->ref());

        GLuint sampler;
        glGenSamplers(1, &sampler);
        glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindSampler(texUnit, sampler);
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

    for (auto& hook : preRenderHooks_) {
        hook();
    }
    for (const Renderable& item : renderables_) {
        setMaterial(item.entity->material);
        setModelTransform(item.transform);
        drawMesh(item.entity->mesh);
    }
    for (auto& hook : postRenderHooks_) {
        hook();
    }
    renderables_.clear();
}



} /* namespace gfx */
} /* namespace zephyr */

