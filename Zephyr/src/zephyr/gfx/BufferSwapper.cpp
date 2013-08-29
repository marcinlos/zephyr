/**
 * @file BufferSwapper.cpp
 */
#include <zephyr/gfx/BufferSwapper.hpp>

namespace zephyr {
namespace gfx {

void BufferSwapper::start() {
    // empty
}

void BufferSwapper::stop() {
    // empty
}

void BufferSwapper::suspend() {
    // empty
}

void BufferSwapper::resume() {
    // empty
}

void BufferSwapper::update() {
    window_.swapBuffers();
}

} /* namespace gfx */
} /* namespace zephyr */
