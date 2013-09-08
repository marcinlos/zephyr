/**
 * @file HackyRenderer.hpp
 */

#ifndef ZEPHYR_GFX_HACKYRENDERER_HPP_
#define ZEPHYR_GFX_HACKYRENDERER_HPP_

#include <zephyr/Context.hpp>
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

    HackyRenderer(Context ctx);

    void update() override;

private:

    void initOpenGL();

    void inputHandler(const core::Message& msg);

    void updateTime();

    const time::ClockManager& clocks;
    const time::Clock& clock;

    time::TaskletScheduler taskletScheduler;
    time::ActionScheduler actionScheduler;

    bool vsync = true;

    std::shared_ptr<struct SceneManager> scene;

    CameraController cameraController;

    int counter;


};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_HACKYRENDERER_HPP_ */
