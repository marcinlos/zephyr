/**
 * @file CameraComponent.hpp
 */

#ifndef ZEPHYR_GFX_CAMERACOMPONENT_HPP_
#define ZEPHYR_GFX_CAMERACOMPONENT_HPP_

#include <zephyr/core/Task.hpp>
#include <zephyr/gfx/Camera.hpp>
#include <zephyr/gfx/Renderer.hpp>


namespace zephyr {
namespace gfx {

using core::Task;


class CameraComponent: public Task {
public:

    /**
     * Name of the uniform block used to store matrices containing camera
     * matrices
     */
    static constexpr char BLOCK_NAME[] = "CameraMatrices";


    CameraComponent(Renderer& renderer, std::shared_ptr<Camera> camera);


    void update() override {
        setCameraPosition();
    }

private:

    void setupCamera();

    void setCameraPosition();

    std::shared_ptr<Camera> camera_;
    Renderer& renderer_;
    UniformManager& uniforms_;
    Viewport& viewport_;
};


} /* namespace gfx */
} /* namespace zephyr */


#endif /* ZEPHYR_GFX_CAMERACOMPONENT_HPP_ */
