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
#include <memory>
#include <queue>

#include <zephyr/input/Key.hpp>

#include <zephyr/input/Position.hpp>


namespace zephyr {
namespace gfx {



class HackyRenderer: public core::Task {
public:

    HackyRenderer(Context ctx);

    void update() override;

private:

    void initOpenGL();

    bool pressed(input::Key key) const {
        return isPressed[static_cast<int>(key)];
    }

    void inputHandler(const core::Message& msg);

    const time::ClockManager& clocks;
    const time::Clock& clock;

    time::TaskletScheduler taskletScheduler;
    time::ActionScheduler actionScheduler;

    bool vsync = true;

    input::Position cursor;


    const static int KEY_NUMBER = static_cast<int>(input::Key::LAST);

    bool isPressed[KEY_NUMBER];

    void updateTime();

    std::shared_ptr<struct SceneManager> scene;

    int counter;


};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_HACKYRENDERER_HPP_ */
