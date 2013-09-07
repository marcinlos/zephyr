/**
 * @file HackyRenderer.hpp
 */

#ifndef ZEPHYR_GFX_HACKYRENDERER_HPP_
#define ZEPHYR_GFX_HACKYRENDERER_HPP_

#include <zephyr/Context.hpp>
#include <zephyr/core/Task.hpp>
#include <zephyr/time/ClockManager.hpp>
#include <zephyr/time/Clock.hpp>
#include <memory>


namespace zephyr {
namespace gfx {

class HackyRenderer: public core::Task {
public:

    HackyRenderer(Context ctx);

    void update() override;

private:
    const time::ClockManager& clocks;
    const time::Clock& clock;

    double prevTime;
    int counter;

    void updateTime();

    std::shared_ptr<struct Data> data_;

    void inputHandler(const core::Message& msg);

};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_HACKYRENDERER_HPP_ */
