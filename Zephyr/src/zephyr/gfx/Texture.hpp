/**
 * @file Texture.hpp
 */

#ifndef ZEPHYR_GFX_TEXTURE_HPP_
#define ZEPHYR_GFX_TEXTURE_HPP_

#include <zephyr/gfx/objects.h>
#include <zephyr/effects/DiamondSquareNoise.hpp>
#include <zephyr/effects/Grid.hpp>
#include <zephyr/effects/NoiseSmoother.hpp>

#include <glimg/Loaders.h>
#include <random>


namespace zephyr {
namespace gfx {


TexturePtr makeTexture();

TexturePtr makeNoise(int size);

TexturePtr loadTexture(const std::string& path);


} /* namespace gfx */
} /* namespace zephyr */


#endif /* ZEPHYR_GFX_TEXTURE_HPP_ */
