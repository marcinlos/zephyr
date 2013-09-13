/**
 * @file SceneManager.hpp
 */

#ifndef ZEPHYR_SCENE_SCENEMANAGER_HPP_
#define ZEPHYR_SCENE_SCENEMANAGER_HPP_

#include <zephyr/scene/SceneGraph.hpp>
#include <zephyr/gfx/Camera.hpp>


namespace zephyr {
namespace scene {

using gfx::Camera;


class SceneManager {
public:




    SceneGraph& scene() {
        return scene_;
    }

private:
    SceneGraph scene_;

    Camera camera_;
    NodePtr cameraNode_;

};

} /* namespace scene */
} /* namespace zephyr */

#endif /* ZEPHYR_SCENE_SCENEMANAGER_HPP_ */
