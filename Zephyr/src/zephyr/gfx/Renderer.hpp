/**
 * @file Renderer.h
 */

#ifndef ZEPHYR_GFX_RENDERER_H_
#define ZEPHYR_GFX_RENDERER_H_

#include <zephyr/core/Task.hpp>
#include <zephyr/gfx/Viewport.hpp>
#include <zephyr/gfx/objects.h>
#include <zephyr/gfx/uniforms.hpp>
#include <zephyr/gfx/UniformManager.hpp>
#include <vector>
#include <unordered_map>

namespace zephyr {
namespace gfx {


class Renderer {
public:

    Renderer();

    void render();

    Viewport& viewport() {
        return viewport_;
    }

    typedef std::function<void ()> PreRenderHook;
    typedef std::function<void ()> PostRenderHook;

    void addPreRenderHook(PreRenderHook hook) {
        preRenderHooks_.push_back(std::move(hook));
    }

    void addPostRenderHook(PostRenderHook hook) {
        postRenderHooks_.push_back(std::move(hook));
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

    void setCulling();
    void setDepthTest();
    void updateViewport();
    void clearBuffers();
    void toggleVSync();
    void drawMesh(const MeshPtr& mesh);
    void setMaterial(const MaterialPtr& material);
    void setModelTransform(const glm::mat4& transform);

    Viewport viewport_;

    bool vsync_ = true;

    std::vector<Renderable> renderables_;

    std::vector<PreRenderHook> preRenderHooks_;
    std::vector<PostRenderHook> postRenderHooks_;

    UniformManager uniforms_;

    ProgramPtr currentProgram_;

    std::unordered_map<GLuint, bool> loaded_;

};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_RENDERER_H_ */
