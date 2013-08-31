/**
 * @file BufferSwapper.hpp
 */

#ifndef ZEPHYR_GFX_EVENTPOLLER_H_
#define ZEPHYR_GFX_EVENTPOLLER_H_

#include <zephyr/core/Task.hpp>
#include <zephyr/window/Window.hpp>

namespace zephyr {
namespace window {

/**
 * Task invoking Window#swapBuffers method for every update.
 */
class EventPoller : public core::Task {
public:

    EventPoller(const Window& window)
    : window_(window)
    { }

    /** Swaps buffer of the associated window */
    void update() override {
        window_.pollEvents();
    };

private:
    /** Window whose buffers are swapped by this task */
    const Window& window_;
};

} /* namespace window */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_EVENTPOLLER_H_ */

