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
#include <zephyr/gfx/FrameBuffer.hpp>
#include <zephyr/resources/ResourceSystem.hpp>
#include <vector>
#include <unordered_map>

namespace zephyr {
namespace gfx {


class Renderer {
public:

    Renderer(ResourceSystem& res);

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
    void setProgram(const ProgramPtr& program);
    void setModelTransform(const glm::mat4& transform);
    void setUniformsForCurrentProgram();

    void hack_skybox();

    ProgramPtr hack_skybox_;
    MeshPtr hack_box_;

    Viewport viewport_;
    ResourceSystem resources_;

    bool vsync_ = true;

    std::unique_ptr<FrameBuffer> gbuffer_;
    ProgramPtr postProcess_;
    MeshPtr screenQuad_;

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
