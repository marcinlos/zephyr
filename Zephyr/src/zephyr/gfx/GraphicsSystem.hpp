/**
 * @file GraphicsSystem.hpp
 */

#ifndef ZEPHYR_GFX_GRAPHICSSYSTEM_HPP_
#define ZEPHYR_GFX_GRAPHICSSYSTEM_HPP_

#include <zephyr/gfx/Renderer.hpp>
#include <zephyr/gfx/DebugDrawer.hpp>
#include <zephyr/util/make_unique.hpp>
#include <zephyr/core/WrapperTask.hpp>
#include <zephyr/Context.hpp>
#include <memory>


namespace zephyr {
namespace gfx {

class GraphicsSystem {
public:


    GraphicsSystem(Context ctx)
    : renderer_ { util::make_unique<Renderer>() }
    , debug_ { util::make_unique<DebugDrawer>(*renderer_) }
    {
        auto invoker = core::wrapAsTask([this]() {
            debug_->update();
            renderer_->render();
        });
        ctx.scheduler.startTask("renderer-invoker", 500000, invoker);

    }

    Renderer& renderer() {
        return *renderer_;
    }

    DebugDrawer& debug() {
        return *debug_;
    }

private:
    std::unique_ptr<Renderer> renderer_;

    std::unique_ptr<DebugDrawer> debug_;

};


} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_GRAPHICSSYSTEM_HPP_ */
