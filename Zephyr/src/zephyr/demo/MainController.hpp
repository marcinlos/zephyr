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

using zephyr::core::Config;
using zephyr::gfx::CameraComponent;
using zephyr::time::Clock;
using zephyr::time::ClockManager;


namespace zephyr {
namespace demo {


class MainController: public core::Task {
public:

    MainController(Root& root);

    void update() override;

private:

    void initCamera();
    void setCameraPosition();
    void submitGeometry();

    Root& root;
    Config& config;

    ClockManager& clocks;
    const Clock& clock;

    std::unique_ptr<CameraComponent> cameraComp;

//    gfx::CameraController cameraController;

//    effects::DayNightCycle dayNightCycle;

};

} /* namespace demo */
} /* namespace zephyr */


#endif /* ZEPHYR_DEMO_MAINCONTROLLER_HPP_ */
