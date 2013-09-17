/**
 * @file Texture.hpp
 */

#ifndef ZEPHYR_GFX_TEXTURE_HPP_
#define ZEPHYR_GFX_TEXTURE_HPP_

#include <zephyr/gfx/objects.h>


namespace zephyr {
namespace gfx {


inline TexturePtr makeTexture() {
    const int N = 256;
    std::uint8_t data[N];
    for (int i = 0; i < N; ++ i) {
        data[i] = i;
    }

    GLuint tex;
    
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_1D, tex);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, N, 0, GL_RED, GL_UNSIGNED_BYTE,
            data);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 0);
    
    glBindTexture(GL_TEXTURE_1D, 0);
    
    return newTexture(tex);
}


} /* namespace gfx */
} /* namespace zephyr */


#endif /* ZEPHYR_GFX_TEXTURE_HPP_ */
