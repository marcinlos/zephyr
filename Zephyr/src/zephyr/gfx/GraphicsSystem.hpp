/**
 * @file GraphicsSystem.hpp
 */

#ifndef ZEPHYR_GFX_GRAPHICSSYSTEM_HPP_
#define ZEPHYR_GFX_GRAPHICSSYSTEM_HPP_

#include <zephyr/core/Scheduler.hpp>
#include <zephyr/resources/ResourceSystem.hpp>
#include <zephyr/gfx/Renderer.hpp>
#include <zephyr/gfx/DebugDrawer.hpp>
#include <zephyr/util/make_unique.hpp>
#include <zephyr/core/WrapperTask.hpp>
#include <zephyr/Context.hpp>
#include <zephyr/core/Config.hpp>
#include <memory>

using zephyr::core::Scheduler;
using zephyr::core::Config;
using zephyr::resources::ResourceSystem;

namespace zephyr {
namespace gfx {


class GraphicsSystem {
public:


    GraphicsSystem(
        Scheduler& scheduler,
        ResourceSystem& resources
    )
    : renderer_ { util::make_unique<Renderer>(resources) }
    , debug_ { util::make_unique<DebugDrawer>(*renderer_, resources) }
    {
        auto invoker = core::wrapAsTask([this]() {
            debug_->update();
            renderer_->render();
        });
        scheduler.startTask("renderer-invoker", 500000, invoker);


        void glimgLoad();
        glimgLoad();

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
