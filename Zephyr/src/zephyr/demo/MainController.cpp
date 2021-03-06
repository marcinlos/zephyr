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

double prevTime;


MainController::MainController(Root& root)
: root(root)
, config(root.config())
, clocks(root.clockManager())
, clock(clocks.getMainClock())
, renderer(root.graphics().renderer())
{
    initCamera();
    initScene();
    initMainTask();

    dayNightCycle = util::make_unique<DayNightCycle>(root);
    taskletScheduler.add([this](double t, double dt){
        dayNightCycle->apply(t);

        glm::vec3 pos = this->root.vars().get<glm::vec3>("sunPos");
        scene::NodePtr sun = landscape->graph.root()->child("sun");
        sun->translateTo(5.0f * pos).translateY(10.0f);

        scene::NodePtr suzanne = landscape->graph.root()->child("suzanne");
        suzanne->rotateX(M_PI * dt / 5);

        cameraBlur->update(dt);

        lights->update();

        return true;
    });

    prevTime = clock.time();

}

void MainController::initCamera() {
    std::cout << "[MainController] Initializing camera\n";

    float FOV = config.get<float>("zephyr.gfx.camera.fov");
    float zNear = config.get<float>("zephyr.gfx.camera.z-near");
    float zFar = config.get<float>("zephyr.gfx.camera.z-far");

    Projection proj { FOV, 1, zNear, zFar };
    glm::vec3 pos { 0, 0, 3.0f };

    camera = std::make_shared<Camera>(proj, pos);
    cameraComp = std::make_shared<CameraComponent>(renderer, camera);
    root.scheduler().startTask("camera-component", 1105, cameraComp);

    cameraController = util::make_unique<gfx::CameraController>(camera, clock);
    core::registerHandler(root.dispatcher(), input::msg::INPUT_SYSTEM,
                cameraController.get(), &CameraController::handle);

    cameraBlur = util::make_unique<effects::CameraMotionBlur>(renderer);
    core::registerHandler(root.dispatcher(), input::msg::INPUT_SYSTEM,
            cameraBlur.get(), &CameraMotionBlur::handle);

    pipelineController = util::make_unique<PipelineController>(renderer);
    core::registerHandler(root.dispatcher(), input::msg::INPUT_SYSTEM,
            pipelineController.get(), &PipelineController::handle);

    lights = util::make_unique<Lights>(renderer, *camera);
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

    glm::vec3 p = 10.0f * camera->forward() + camera->pos;
    root.graphics().debug().addBox(glm::translate(p));

    landscape->graph.update();
    submitGeometry();

    std::cout << "FPS: " << 1 / (time - prevTime) << std::endl;
    prevTime = time;
}



} /* namespace demo */
} /* namespace zephyr */
