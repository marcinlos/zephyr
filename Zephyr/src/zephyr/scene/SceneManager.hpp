/**
 * @file SceneManager.hpp
 */

#ifndef ZEPHYR_SCENE_SCENEMANAGER_HPP_
#define ZEPHYR_SCENE_SCENEMANAGER_HPP_

#include <zephyr/scene/SceneGraph.hpp>
#include <zephyr/gfx/Camera.hpp>
#include <zephyr/gfx/Renderer.hpp>


namespace zephyr {
namespace scene {

using gfx::Camera;
using gfx::EntityPtr;



class SceneManager {
public:

    struct RenderItem {
        NodePtr node;
        EntityPtr entity;
    };




    std::vector<RenderItem>& renderables() {
        return items_;
    }

    SceneGraph& scene() {
        return scene_;
    }

    Camera& camera() {
        return camera_;
    }

    NodePtr cameraNode() {
        return cameraNode_;
    }

private:
    std::vector<RenderItem> items_;
    SceneGraph scene_;

    Camera camera_;
    NodePtr cameraNode_;

};

} /* namespace scene */
} /* namespace zephyr */

#endif /* ZEPHYR_SCENE_SCENEMANAGER_HPP_ */
