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


namespace zephyr {
namespace gfx {


class HackyRenderer: public core::Task {
public:

    HackyRenderer(Root& root);

    void update() override;

private:

    void updateTime();

    const time::ClockManager& clocks;
    const time::Clock& clock;

    time::TaskletScheduler taskletScheduler;
    time::ActionScheduler actionScheduler;

    std::shared_ptr<struct SceneManager_> scene;

    CameraController cameraController;

    int counter;


};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_HACKYRENDERER_HPP_ */
