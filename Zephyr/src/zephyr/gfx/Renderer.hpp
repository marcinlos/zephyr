/**
 * @file Renderer.h
 */

#ifndef ZEPHYR_GFX_RENDERER_H_
#define ZEPHYR_GFX_RENDERER_H_

#include <zephyr/core/Task.hpp>
#include <zephyr/gfx/Viewport.hpp>


namespace zephyr {
namespace gfx {

class Renderer:  public core::Task {
public:

    Renderer();

    void update() override;

    Viewport& viewport() {
        return viewport_;
    }

private:

    Viewport viewport_;

    bool vsync_;

    void setCulling();
    void setDepthTest();
    void updateViewport();
    void clearBuffers();
    void toggleVSync();
};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_RENDERER_H_ */
