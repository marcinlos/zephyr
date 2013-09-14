/**
 * @file Renderer.h
 */

#ifndef ZEPHYR_GFX_RENDERER_H_
#define ZEPHYR_GFX_RENDERER_H_

#include <zephyr/core/Task.hpp>
#include <zephyr/gfx/Viewport.hpp>
#include <zephyr/gfx/objects.h>
#include <zephyr/gfx/uniforms.hpp>
#include <vector>
#include <unordered_map>

namespace zephyr {
namespace gfx {


class UniformManager {
public:

    void uniform(const std::string& name, UniformPtr uniform) {
        uniforms_[name] = std::move(uniform);
    }

    Uniform* uniform(const std::string& name) {
        auto it = uniforms_.find(name);
        if (it != end(uniforms_)) {
            return it->second.get();
        } else {
            return nullptr;
        }
    }

    GLuint registerUniformBlock(const std::string& name, GLuint buffer,
            std::size_t size) {
        GLuint index = nextBindingIndex_ ++;
        blocks_[name] = index;
        buffers_[name] = buffer;
        glBindBufferRange(GL_UNIFORM_BUFFER, index, buffer, 0, size);
        return index;
    }

    GLuint createUniformBlock(const std::string& name,  GLsizei size) {
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STREAM_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        return registerUniformBlock(name, buffer, size);
    }

    GLint uniformBlockBindingIndex(const std::string& name) {
        auto it = blocks_.find(name);
        if (it != end(blocks_)) {
            return it->second;
        } else {
            return -1;
        }
    }

    void fillUniformBlock(const std::string& name, void* data,
            std::ptrdiff_t offset, std::size_t size) {
        GLuint buffer = buffers_[name];
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

private:
    typedef std::unordered_map<std::string, UniformPtr> UniformMap;

    UniformMap uniforms_;

    std::unordered_map<std::string, GLuint> blocks_;

    std::unordered_map<std::string, GLuint> buffers_;

    GLint nextBindingIndex_ = 1;

};



class Renderer {
public:

    Renderer();

    void render();

    Viewport& viewport() {
        return viewport_;
    }

    void submit(Renderable renderable) {
        renderables_.push_back(std::move(renderable));
    }

    UniformManager& uniforms() {
        return uniforms_;
    }

private:

    bool isLoaded(const ProgramPtr& program) const {
        auto it = loaded_.find(program->ref());
        if (it != end(loaded_)) {
            return it->second;
        } else {
            return false;
        }
    }

    bool markAsLoaded(const ProgramPtr& program) {
        bool wasLoaded = isLoaded(program);
        loaded_[program->ref()] = true;
        return wasLoaded;
    }

    Viewport viewport_;

    bool vsync_ = true;

    std::vector<Renderable> renderables_;

    UniformManager uniforms_;

    ProgramPtr currentProgram_;

    std::unordered_map<GLuint, bool> loaded_;

    void setCulling();
    void setDepthTest();
    void updateViewport();
    void clearBuffers();
    void toggleVSync();
    void drawMesh(const MeshPtr& mesh);
    void setMaterial(const MaterialPtr& material);
    void setModelTransform(const glm::mat4& transform);
};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_RENDERER_H_ */
