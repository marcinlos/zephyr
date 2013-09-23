/**
 * @file CameraComponent.cpp
 */
#include <zephyr/gfx/CameraComponent.hpp>

namespace zephyr {
namespace gfx {

CameraComponent::CameraComponent(Renderer& renderer,
        std::shared_ptr<Camera> camera)
: camera_ { std::move(camera) }
, renderer_(renderer)
, uniforms_(renderer.uniforms())
, viewport_(renderer.viewport())
{
    setupCamera();
}

void CameraComponent::setupCamera() {
    using std::placeholders::_1;

    auto ratioUpdate = std::bind(&Camera::adjustRatio, camera_, _1);
    viewport_.listener(ratioUpdate);

    std::size_t blockSize = 2 * sizeof(glm::mat4);
    uniforms_.createBlock(BLOCK_NAME, blockSize);
}

void CameraComponent::setCameraPosition() {
    glm::mat4 viewMatrix = camera_->viewMatrix();
    glm::mat4 projMatrix = camera_->projectionMatrix();

    void* viewData = glm::value_ptr(viewMatrix);
    void* projData = glm::value_ptr(projMatrix);

    std::size_t size = sizeof(glm::mat4);
    uniforms_.fillBlock(BLOCK_NAME, viewData, 0, size);
    uniforms_.fillBlock(BLOCK_NAME, projData, size, size);
}


constexpr char CameraComponent::BLOCK_NAME[];

} /* namespace gfx */
} /* namespace zephyr */
