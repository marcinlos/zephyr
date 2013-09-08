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
#include <zephyr/input/ButtonEvent.hpp>
#include <zephyr/input/Position.hpp>


namespace zephyr {
namespace gfx {


class InputState {
public:

    bool& operator [] (input::Key key) {
        return keyState[asInt(key)];
    }

    bool operator [] (input::Key key) const {
        return keyState[asInt(key)];
    }

    bool& operator [] (input::Button button) {
        return buttonState[asInt(button)];
    }

    bool operator [] (input::Button button) const {
        return buttonState[asInt(button)];
    }

    input::Position mouse() const {
        return cursorPos;
    }

    void mouse(input::Position position) {
        cursorPos = position;
    }


private:
    int asInt(input::Key key) const {
        return static_cast<int>(key);
    }

    int asInt(input::Button button) const {
        return static_cast<int>(button);
    }

    const static int KEY_NUMBER = static_cast<int>(input::Key::LAST);
    const static int BUTTON_NUMBER = static_cast<int>(input::Button::LAST);

    bool keyState[KEY_NUMBER];
    bool buttonState[BUTTON_NUMBER];

    input::Position cursorPos;
};


class HackyRenderer: public core::Task {
public:

    HackyRenderer(Context ctx);

    void update() override;

private:

    void initOpenGL();

    void inputHandler(const core::Message& msg);

    const time::ClockManager& clocks;
    const time::Clock& clock;

    time::TaskletScheduler taskletScheduler;
    time::ActionScheduler actionScheduler;

    bool vsync = true;

    InputState input;

    void updateTime();

    std::shared_ptr<struct SceneManager> scene;

    int counter;


};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_HACKYRENDERER_HPP_ */
