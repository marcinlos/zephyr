/**
 * @file BufferSwapper.hpp
 */

#ifndef ZEPHYR_GFX_BUFFERSWAPPER_H_
#define ZEPHYR_GFX_BUFFERSWAPPER_H_

#include <zephyr/core/Task.hpp>
#include <zephyr/gfx/Window.hpp>

namespace zephyr {
namespace gfx {

/**
 * Task invoking Window#swapBuffers method for every update.
 */
class BufferSwapper : public core::Task {
public:

    BufferSwapper(const Window& window)
    : window_(window)
    { }

    /** Swaps buffer of the associated window */
    void update() override {
        window_.swapBuffers();
    };

private:
    /** Window whose buffers are swapped by this task */
    const Window& window_;
};

}
}

#endif /* ZEPHYR_GFX_BUFFERSWAPPER_H_ */

