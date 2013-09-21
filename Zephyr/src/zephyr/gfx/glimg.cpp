/**
 * @file glimg.cpp
 */

#include <iostream>
#include <string>
#include <glimg/glimg.h>
#include <glload/gl_3_3.hpp>
#include <glload/gl_load.hpp>

using namespace glimg;

namespace zephyr {
namespace gfx {

void glimgLoad() {
    std::clog << "[glload] Loading functions..." << std::endl;
    if (!glload::LoadFunctions()) {
        std::terminate();
    }
}


unsigned int glimgCreateTexture(ImageSet* img) {
    return CreateTexture(img, 0);
}


} /* namespace gfx */
} /* namespace zephyr */




