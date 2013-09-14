/**
 * @file HackyRenderer.cpp
 */

#include <zephyr/gfx/HackyRenderer.hpp>
#include <zephyr/resources/ResourceManager.hpp>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <zephyr/gfx/Camera.hpp>

#include <zephyr/gfx/star.hpp>

#include <zephyr/gfx/objects.h>
#include <zephyr/gfx/Shader.hpp>
#include <zephyr/gfx/Program.hpp>
#include <zephyr/gfx/Mesh.hpp>
#include <zephyr/scene/SceneGraph.hpp>
#include <zephyr/gfx/SceneBuilder.hpp>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <unordered_map>
#include <list>


namespace zephyr {
namespace gfx {


HackyRenderer::HackyRenderer(Root& root)
: root(root)
, camera {
    Projection { FOV, 1, zNear, zFar },
    glm::vec3 { 0.0f, 0.0f, 3.0f }
}
, clocks(root.clockManager())
, clock(clocks.getMainClock())
, builder(std::make_shared<SceneBuilder>())
, cameraController(camera, clock)
, dayNightCycle { root.graphics().renderer() }
{
    std::cout << "[Hacky] Initializing hacky renderer" << std::endl;

    core::registerHandler(root.dispatcher(), input::msg::INPUT_SYSTEM,
            &cameraController, &CameraController::handle);

    Renderer& renderer = root.graphics().renderer();
    using std::placeholders::_1;
    auto ratioUpdate = std::bind(&Camera::adjustRatio, &camera, _1);
    renderer.viewport().listener(ratioUpdate);

    std::size_t size = 2 * sizeof(glm::mat4);
    renderer.uniforms().createUniformBlock("CameraMatrices", size);

    taskletScheduler.add([this](double t, double dt) {
        this->builder->graph.root()->firstChild()->rotateX(M_PI * dt / 5);
        return true;
    });

    taskletScheduler.add([this](double time, double){
        dayNightCycle.apply(time);
        return true;
    });
}


void HackyRenderer::updateTime() {
    double time = clock.time();
    taskletScheduler.update(time, clock.dt());
    actionScheduler.update(time);
}

void HackyRenderer::setCameraPosition() {
    Renderer& renderer = root.graphics().renderer();
    glm::mat4 viewMatrix = camera.viewMatrix();
    glm::mat4 projMatrix = camera.projectionMatrix();

    UniformManager& uniforms = renderer.uniforms();

    void* viewData = glm::value_ptr(viewMatrix);
    void* projData = glm::value_ptr(projMatrix);

    std::size_t size = sizeof(glm::mat4);
    uniforms.fillUniformBlock("CameraMatrices", viewData, 0, size);
    uniforms.fillUniformBlock("CameraMatrices", projData, size, size);
}

void HackyRenderer::submitGeometry() {
    auto submit = [this](const SceneBuilder::Item& item) {
        Renderer& renderer = root.graphics().renderer();
        renderer.submit(Renderable {
            item.entity,
            item.node->globalTransform()
        });
    };
    std::for_each(begin(builder->items), end(builder->items), submit);
}

void HackyRenderer::update() {
    updateTime();
    builder->graph.update();
    cameraController.update();
    setCameraPosition();
    submitGeometry();
}


} /* namespace gfx */
} /* namespace zephyr */

