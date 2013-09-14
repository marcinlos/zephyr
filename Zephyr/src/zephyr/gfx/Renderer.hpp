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

    void registerUniformBlock(const std::string& name) {
        blocks_[name] = nextBindingIndex_ ++;
    }

    GLint uniformBlockBindingIndex(const std::string& name) {
        auto it = blocks_.find(name);
        if (it != end(blocks_)) {
            return it->second;
        } else {
            return -1;
        }
    }

private:
    typedef std::unordered_map<std::string, UniformPtr> UniformMap;

    UniformMap uniforms_;

    std::unordered_map<std::string, GLuint> blocks_;

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

    Viewport viewport_;

    bool vsync_ = true;

    std::vector<Renderable> renderables_;

    UniformManager uniforms_;

    ProgramPtr currentProgram_;

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
