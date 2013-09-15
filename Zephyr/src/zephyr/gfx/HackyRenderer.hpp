/**
 * @file HackyRenderer.hpp
 */

#ifndef ZEPHYR_GFX_HACKYRENDERER_HPP_
#define ZEPHYR_GFX_HACKYRENDERER_HPP_

#include <zephyr/Root.hpp>
#include <zephyr/core/Task.hpp>
#include <zephyr/time/ClockManager.hpp>
#include <zephyr/time/Clock.hpp>
#include <zephyr/time/ActionScheduler.hpp>
#include <zephyr/time/TaskletScheduler.hpp>
#include <zephyr/gfx/CameraController.hpp>
#include <memory>
#include <queue>

#include <zephyr/effects/DayNightCycle.hpp>


namespace zephyr {
namespace gfx {

/*
class HackyRenderer: public core::Task {
public:

    HackyRenderer(Root& root);

    void update() override;

private:

    static constexpr float FOV = 60.0f;
    static constexpr float zNear = 1.0f;
    static constexpr float zFar = 100.0f;

    void updateTime();
    void setCameraPosition();
    void submitGeometry();

    Root& root;

    Camera camera;

    const time::ClockManager& clocks;
    const time::Clock& clock;

    time::TaskletScheduler taskletScheduler;
    time::ActionScheduler actionScheduler;

    CameraController cameraController;

    effects::DayNightCycle dayNightCycle;

};*/

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_HACKYRENDERER_HPP_ */
