/**
 * @file Renderer.h
 */

#ifndef ZEPHYR_GFX_RENDERER_H_
#define ZEPHYR_GFX_RENDERER_H_

#include <zephyr/core/Task.hpp>
#include <zephyr/gfx/Viewport.hpp>
#include <zephyr/gfx/objects.h>
#include <vector>
#include <unordered_map>

namespace zephyr {
namespace gfx {


class UniformManager {
public:

    void global(const std::string& name, UniformPtr uniform) {
        uniforms_[name] = std::move(uniform);
    }

    Uniform& operator [] (const std::string& name) {
        return *(uniforms_[name]);
    }


private:
    typedef std::unordered_map<std::string, UniformPtr> UniformMap;

    UniformMap uniforms_;
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

private:

    Viewport viewport_;

    bool vsync_ = true;

    std::vector<Renderable> renderables_;

    UniformManager uniforms_;

    void setCulling();
    void setDepthTest();
    void updateViewport();
    void clearBuffers();
    void toggleVSync();
};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_RENDERER_H_ */
