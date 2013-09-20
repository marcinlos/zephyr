/*
 * SceneBuilder.hpp
 */

#ifndef ZEPHYR_GFX_SCENEBUILDER_HPP_
#define ZEPHYR_GFX_SCENEBUILDER_HPP_

#include <zephyr/scene/SceneGraph.hpp>
#include <zephyr/resources/ResourceSystem.hpp>
#include <zephyr/gfx/objects.h>
#include <vector>


using zephyr::scene::NodePtr;
using zephyr::scene::SceneGraph;
using zephyr::gfx::EntityPtr;
using zephyr::resources::ResourceSystem;


namespace zephyr {
namespace demo {

struct LandscapeScene {

    struct Item {
        NodePtr node;
        EntityPtr entity;
    };

    ResourceSystem& res;

    std::vector<Item> items;
    SceneGraph graph;

    LandscapeScene(ResourceSystem& res)
    : res(res)
    {
        createResources();
        build();
    }

private:
    void build();
    void createResources();
};


} /* namespace demo */
} /* namespace zephyr */


#endif /* ZEPHYR_GFX_SCENEBUILDER_HPP_ */
