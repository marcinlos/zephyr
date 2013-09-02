/**
 * @file HackyRenderer.hpp
 */

#ifndef ZEPHYR_GFX_HACKYRENDERER_HPP_
#define ZEPHYR_GFX_HACKYRENDERER_HPP_

#include <zephyr/Context.hpp>
#include <zephyr/core/Task.hpp>
#include <zephyr/time/ClockManager.hpp>

#include <zephyr/time/TimeSource.hpp>
#include <memory>


namespace zephyr {
namespace gfx {

class HackyRenderer: public core::Task {
public:

    HackyRenderer(Context ctx);

    void update() override;

private:
    time::ClockManager& clocks;
    double prevTime;
    int counter;

    time::TimeSource clock;

    void updateTime();

    std::shared_ptr<struct Data> data_;

};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_HACKYRENDERER_HPP_ */
