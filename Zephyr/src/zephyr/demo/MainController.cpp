/**
 * @file MainController.cpp
 */

#include <zephyr/demo/MainController.hpp>
#include <zephyr/util/make_unique.hpp>
#include <functional>

namespace zephyr {
namespace demo {

using namespace gfx;

MainController::MainController(Root& root)
: root(root)
, config(root.config())
, clocks(root.clockManager())
, clock(clocks.getMainClock())
//, builder(std::make_shared<LandscapeScene>())
//, cameraController(camera, clock)
//, dayNightCycle { root }
{
    initCamera();
//    core::registerHandler(root.dispatcher(), input::msg::INPUT_SYSTEM,
//            &cameraController, &CameraController::handle);

//    taskletScheduler.add([this](double t, double dt) {
//        this->builder->graph.root()->firstChild()->rotateX(M_PI * dt / 5);
//        return true;
//    });

//    taskletScheduler.add([this](double time, double){
//        dayNightCycle.apply(time);
//        return true;
//    });

//    taskletScheduler.add([this](double, double) {
//        glm::vec3 pos = this->root.vars().get<glm::vec3>("sunPos");
//        scene::NodePtr sun = builder->graph.root()->child("sun");
//        sun->translateTo(5.0f * pos).translateY(4.0f);
//        return true;
//    });
}

void MainController::initCamera() {
    std::cout << "[MainController] Initializing camera\n";

    float FOV = config.get<float>("zephyr.gfx.camera.fov");
    float zNear = config.get<float>("zephyr.gfx.camera.z-near");
    float zFar = config.get<float>("zephyr.gfx.camera.z-far");

    Renderer& renderer = root.graphics().renderer();
    Projection proj { FOV, zNear, zFar };
    glm::vec3 pos { 0, 0, 0 };

    std::shared_ptr<Camera> camera = std::make_shared<Camera>(proj, pos);

    cameraComp = util::make_unique<CameraComponent>(renderer, camera);
}


void MainController::submitGeometry() {
//    auto submit = [this](const scene::SceneManager::RenderItem& item) {
//        Renderer& renderer = root.graphics().renderer();
//        renderer.submit(Renderable {
//            item.entity,
//            item.node->globalTransform()
//        });
//    };
//    std::for_each(begin(builder->items), end(builder->items), submit);
}

void MainController::update() {
//    builder->graph.update();
//    cameraController.update();
    submitGeometry();
}



} /* namespace demo */
} /* namespace zephyr */
