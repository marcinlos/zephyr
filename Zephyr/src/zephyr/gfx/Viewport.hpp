/*
 * Viewport.hpp
 *
 *  Created on: Sep 12, 2013
 *      Author: los
 */

#ifndef VIEWPORT_HPP_
#define VIEWPORT_HPP_

#include <functional>


namespace zephyr {
namespace gfx {

class Viewport {
public:

    typedef std::function<void (float)> Listener;

    Viewport()
    : Viewport { 0, 0, -1, -1 }
    { }

    Viewport(int x, int y, int width, int height)
    : x_(x), y_(y), width_(width), height_(height)
    { }

    int left() const {
        return x_;
    }

    int top() const {
        return y_;
    }

    int width() const {
        return width_;
    }

    int height() const {
        return height_;
    }

    void set(int x, int y, int width, int height) {
        this->x_ = x;
        this->y_ = y;
        this->width_ = width;
        this->height_ = height;

        if (listener_) {
            listener_(aspectRatio());
        }
    }

    float aspectRatio() const {
        return width_ / static_cast<float>(height_);
    }

    void listener(Listener listener) {
        this->listener_ = std::move(listener);
    }


private:
    int x_;
    int y_;
    int width_;
    int height_;
    Listener listener_;
};

} /* namespace gfx */
} /* namespace zephyr */
#endif /* VIEWPORT_HPP_ */
