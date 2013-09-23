/**
 * @file MainController.hpp
 */

#ifndef ZEPHYR_DEMO_MAINCONTROLLER_HPP_
#define ZEPHYR_DEMO_MAINCONTROLLER_HPP_

#include <zephyr/Root.hpp>
#include <zephyr/core/Task.hpp>
#include <zephyr/time/ClockManager.hpp>
#include <zephyr/time/TaskletScheduler.hpp>
#include <zephyr/time/ActionScheduler.hpp>
#include <zephyr/gfx/CameraComponent.hpp>
#include <zephyr/effects/DayNightCycle.hpp>
#include <zephyr/effects/CameraMotionBlur.hpp>

// temporary
#include <zephyr/demo/LandscapeScene.hpp>
#include <zephyr/gfx/CameraController.hpp>
#include <zephyr/demo/PipelineController.hpp>
#include <zephyr/demo/Lights.hpp>

using zephyr::core::Config;
using zephyr::gfx::Renderer;
using zephyr::gfx::Camera;
using zephyr::gfx::CameraComponent;
using zephyr::gfx::CameraController;
using zephyr::time::Clock;
using zephyr::time::ClockManager;
using zephyr::time::TaskletScheduler;
using zephyr::effects::DayNightCycle;
using zephyr::effects::CameraMotionBlur;

namespace zephyr {
namespace demo {


class MainController
        : public core::Task
        , public std::enable_shared_from_this<MainController> {
public:

    MainController(Root& root);

    void update() override;

private:

    void initCamera();
    void initScene();
    void initMainTask();
    void setCameraPosition();
    void submitGeometry();

    Root& root;

    Config& config;
    ClockManager& clocks;
    const Clock& clock;
    Renderer& renderer;
    TaskletScheduler taskletScheduler;

    std::shared_ptr<Camera> camera;
    std::shared_ptr<CameraComponent> cameraComp;

    // temporary
    std::unique_ptr<LandscapeScene> landscape;

    std::unique_ptr<CameraController> cameraController;
    std::unique_ptr<PipelineController> pipelineController;
    std::unique_ptr<Lights> lights;

    std::unique_ptr<CameraMotionBlur> cameraBlur;
    std::unique_ptr<DayNightCycle> dayNightCycle;

};

} /* namespace demo */
} /* namespace zephyr */


#endif /* ZEPHYR_DEMO_MAINCONTROLLER_HPP_ */
