/**
 * @file Renderer.cpp
 */

#include <zephyr/gfx/Renderer.hpp>
#include <zephyr/gfx/MeshBuilder.hpp>
#include <zephyr/util/make_unique.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>


namespace zephyr {
namespace gfx {


static const GLfloat screenQuadVertices[] = {
    -1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
};


Renderer::Renderer(ResourceSystem& res) {
    glewInit();
    glfwSwapInterval(vsync_);

    setCulling();
    setDepthTest();

    updateViewport();

    screenQuad_ = MeshBuilder()
            .setBuffer(screenQuadVertices).attribute(0, 3)
            .create();

    postProcess_ = res.program("post");
}

void Renderer::updateViewport() {
    int w, h;
    GLFWwindow* window = glfwGetCurrentContext();
    glfwGetFramebufferSize(window, &w, &h);
    bool changed = w != viewport_.width() || h != viewport_.height();
    viewport_.set(0, 0, w, h);
    glViewport(0, 0, w, h);

    if (changed) {
        gbuffer_ = util::make_unique<FrameBuffer>(4, w, h);
        uniforms_.set4ui("viewport", 0, 0, w, h);
    }

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

struct TextureBinder {
public:

    TextureBinder(ProgramPtr program)
    : nextFreeUnit_ { 0 }
    , program_ { std::move(program) }
    { }

    TextureBinder& bind(GLint index, GLuint texture) {
        glActiveTexture(GL_TEXTURE0 + nextFreeUnit_);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(index, nextFreeUnit_);

        GLuint sampler;
        glGenSamplers(1, &sampler);
        glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindSampler(nextFreeUnit_, sampler);

        glActiveTexture(GL_TEXTURE0);
        ++ nextFreeUnit_;
        return *this;
    }

    TextureBinder& bind(const std::string& name, GLint texture) {
        GLint uniform = program_->uniformLocation(name);
        if (uniform >= 0) {
            bind(uniform, texture);
        }
        return *this;
    }

private:
    GLint nextFreeUnit_;
    ProgramPtr program_;
};

void Renderer::setUniformsForCurrentProgram() {
    for (const auto& needed : currentProgram_->uniforms()) {
        const std::string& name = needed.first;
        if (Uniform* value = uniforms_.get(name)) {
            value->set(needed.second);
        }
    }
}

void Renderer::setProgram(const ProgramPtr& program) {
    if (currentProgram_ != program) {
        glUseProgram(program->ref());
        currentProgram_ = program;
        if (!markAsLoaded(currentProgram_)) {
            for (const auto& blocks : currentProgram_->uniformBlocks()) {
                const std::string& name = blocks.first;
                GLuint index = blocks.second;
                GLuint bindingIndex = uniforms_.blockBindingIndex(name);
                currentProgram_->bindBlock(index, bindingIndex);
            }
        }
    }
}

void Renderer::setMaterial(const MaterialPtr& material) {
    setProgram(material->program);
    setUniformsForCurrentProgram();

    for (const auto& local : material->uniforms) {
        const std::string& name = local.first;
        GLint slot = currentProgram_->uniformLocation(name);
        if (slot >= 0) {
            local.second->set(slot);
        }
    }
    TextureBinder binder { currentProgram_ };

    for (const auto& texPair : material->textures) {
        GLint samplerUniform = texPair.first;
        if (samplerUniform < 0) continue;

        binder.bind(samplerUniform, texPair.second->ref());
    }
}


void Renderer::setModelTransform(const glm::mat4& transform) {
    GLint location = currentProgram_->uniformLocation("modelMatrix");
    auto data = glm::value_ptr(transform);
    glUniformMatrix4fv(location, 1, GL_FALSE, data);
}


void Renderer::render() {
    gbuffer_->bind();
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

    gbuffer_->unbind();
    updateViewport();
    clearBuffers();

    setProgram(postProcess_);
    setUniformsForCurrentProgram();

    TextureBinder binder { postProcess_ };
    binder
        .bind("renderedTexture", gbuffer_->get(0))
        .bind("normalTexture", gbuffer_->get(1))
        .bind("specularTexture", gbuffer_->get(2))
        .bind("depthTexture", gbuffer_->get(3))
        ;

    drawMesh(screenQuad_);
}



} /* namespace gfx */
} /* namespace zephyr */

