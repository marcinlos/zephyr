/**
 * @fiile FrameBuffer.cpp
 */

#include <zephyr/gfx/FrameBuffer.hpp>
#include <stdexcept>


namespace zephyr {
namespace gfx {

FrameBuffer::FrameBuffer(std::size_t targets, int width, int height)
: targets_ { targets }
, textures_(targets)
{
    glGenFramebuffers(1, &frameBuffer_);
    bind();

    glGenTextures(targets, textures_.data());

    int i = 0;
    for (auto tex : textures_) {
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
                GL_FLOAT, nullptr);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, tex, 0);
        ++ i;
    }

//    glGenRenderbuffers(1, &depthBuffer);
//    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    glGenTextures(1, &depthTexture_);
    glBindTexture(GL_TEXTURE_2D, depthTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0,
            GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture_, 0);

    std::vector<GLenum> buffers(targets);
    for (std::size_t i = 0; i < targets; ++ i) {
        buffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    glDrawBuffers(buffers.size(), buffers.data());
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Error while creating GBuffer");
    }
    unbind();
}

void FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer_);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


} /* namespace gfx */
} /* namespace zephyr */
