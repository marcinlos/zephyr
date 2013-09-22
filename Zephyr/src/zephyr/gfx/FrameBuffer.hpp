/**
 * @file FrameBuffer.hpp
 */

#ifndef ZEPHYR_GFX_GBUFFER_HPP_
#define ZEPHYR_GFX_FRAMEBUFFER_HPP_

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>


namespace zephyr {
namespace gfx {

class FrameBuffer {
public:
    FrameBuffer(std::size_t targets, int width, int height);

    GLuint frameBuffer() const {
        return frameBuffer_;
    }

    void bind();

    void unbind();

    GLuint get(int n) const {
        return textures_[n];
    }

    GLuint operator [] (int n) const {
        return get(n);
    }

    GLuint depth() const {
        return depthTexture_;
    }

    std::size_t targetCount() const {
        return targets_;
    }

private:
    std::size_t targets_;

    GLuint frameBuffer_;

    std::vector<GLuint> textures_;
    GLuint depthTexture_;
};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_FRAMEBUFFER_HPP_ */
