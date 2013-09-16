/**
 * @file MainController.cpp
 */

#include <zephyr/demo/MainController.hpp>
#include <zephyr/util/make_unique.hpp>
#include <functional>

#include <zephyr/gfx/uniform_values.hpp>

namespace zephyr {
namespace demo {

using namespace gfx;

MainController::MainController(Root& root)
: root(root)
, config(root.config())
, clocks(root.clockManager())
, clock(clocks.getMainClock())
, renderer(root.graphics().renderer())
{
    std::cout << "Size: " << sizeof(gfx::uuniform) << std::endl;
    initCamera();
    initScene();
    initMainTask();

    dayNightCycle = util::make_unique<DayNightCycle>(root);
    taskletScheduler.add([this](double time, double){
        dayNightCycle->apply(time);
        return true;
    });

    taskletScheduler.add([this](double, double) {
        glm::vec3 pos = this->root.vars().get<glm::vec3>("sunPos");
        scene::NodePtr sun = landscape->graph.root()->child("sun");
        sun->translateTo(5.0f * pos).translateY(4.0f);
        return true;
    });
}

void MainController::initCamera() {
    std::cout << "[MainController] Initializing camera\n";

    float FOV = config.get<float>("zephyr.gfx.camera.fov");
    float zNear = config.get<float>("zephyr.gfx.camera.z-near");
    float zFar = config.get<float>("zephyr.gfx.camera.z-far");

    Projection proj { FOV, 1, zNear, zFar };
    glm::vec3 pos { 0, 0, 3.0f };

    std::shared_ptr<Camera> camera = std::make_shared<Camera>(proj, pos);
    cameraComp = std::make_shared<CameraComponent>(renderer, camera);
    root.scheduler().startTask("camera-component", 1105, cameraComp);

    cameraController = util::make_unique<gfx::CameraController>(camera, clock);
    core::registerHandler(root.dispatcher(), input::msg::INPUT_SYSTEM,
                cameraController.get(), &CameraController::handle);
}


void MainController::initScene() {
    landscape = util::make_unique<LandscapeScene>(root.resources());
}


void MainController::initMainTask() {
    struct {
        void operator () (MainController*) { }
    } nop;
    std::shared_ptr<MainController> this_(this, nop);
    root.scheduler().startTask("main-controller", 1100, std::move(this_));
}


void MainController::submitGeometry() {
    auto submit = [this](const LandscapeScene::Item& item) {
        renderer.submit(Renderable {
            item.entity,
            item.node->globalTransform()
        });
    };
    std::for_each(begin(landscape->items), end(landscape->items), submit);
}


void MainController::update() {
    double time = clock.time();
    taskletScheduler.update(time, clock.dt());
    cameraController->update();
    landscape->graph.update();
    submitGeometry();
}



} /* namespace demo */
} /* namespace zephyr */
