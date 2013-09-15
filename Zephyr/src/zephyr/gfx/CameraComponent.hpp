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


    CameraComponent(Renderer& renderer, std::shared_ptr<Camera> camera)
    : camera_ { std::move(camera) }
    , renderer_(renderer)
    , uniforms_(renderer.uniforms())
    , viewport_(renderer.viewport())
    { }


    void update() override {
        setCameraPosition();
    }


private:

    void setupCamera() {
        using std::placeholders::_1;

        auto ratioUpdate = std::bind(&Camera::adjustRatio, camera_, _1);
        viewport_.listener(ratioUpdate);

        std::size_t blockSize = 2 * sizeof(glm::mat4);
        uniforms_.createUniformBlock(BLOCK_NAME, blockSize);
    }


    void setCameraPosition() {
        glm::mat4 viewMatrix = camera_->viewMatrix();
        glm::mat4 projMatrix = camera_->projectionMatrix();

        void* viewData = glm::value_ptr(viewMatrix);
        void* projData = glm::value_ptr(projMatrix);

        std::size_t size = sizeof(glm::mat4);
        uniforms_.fillUniformBlock(BLOCK_NAME, viewData, 0, size);
        uniforms_.fillUniformBlock(BLOCK_NAME, projData, size, size);
    }

    std::shared_ptr<Camera> camera_;
    Renderer& renderer_;
    UniformManager& uniforms_;
    Viewport& viewport_;
};


} /* namespace gfx */
} /* namespace zephyr */


#endif /* ZEPHYR_GFX_CAMERACOMPONENT_HPP_ */
