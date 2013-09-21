/**
 * @file Texture.cpp
 */

#include <zephyr/gfx/Texture.hpp>
#include <glimg/glimg.h>

namespace zephyr {
namespace gfx {

TexturePtr makeTexture() {
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

    return newTexture(tex, TexDim::_1D, 256);
}


TexturePtr makeNoise(int size) {
    using effects::diamond::noise;
    std::default_random_engine engine(std::time(nullptr));
    std::uniform_real_distribution<float> rand(0, 1);

    int n = 1 << size;

    auto data = noise({ size, 0.7f });
    std::vector<float> d;
    d.reserve(n * n);

    for (int i = 0; i < n; ++ i) {
        auto from = begin(data) + (i + 1) * (n + 1);
        auto to = from + n;
        copy(from, to, back_inserter(d));
    }

    auto extrema = std::minmax_element(begin(data), end(data));
    float min = *extrema.first;
    float max = *extrema.second;

    std::for_each(begin(d), end(d), [=, &engine](float& val) mutable {
        float x = (val - min) / (max - min);
        float arg = 0.8 * x + 0.2f * rand(engine);
        float v = std::sin(arg * 2 * M_PI);
        val = v * v;
    });

    GLuint tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, n, n, 0, GL_RED, GL_FLOAT, &d[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    return newTexture(tex, TexDim::_2D, n, n);
}


unsigned int glimgCreateTexture(glimg::ImageSet* img);


TexturePtr loadTexture(const std::string& path) {
    using namespace glimg;
    std::unique_ptr<ImageSet> image { loaders::stb::LoadFromFile(path) };
    auto dim = image->GetDimensions();
    TexDim dimensions = static_cast<TexDim>(dim.numDimensions);

    GLuint tex = glimgCreateTexture(image.get());

//    glGenTextures(1, &tex);
//    glBindTexture(GL_TEXTURE_2D, tex);
//
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dim.width, dim.height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, image->GetImage(0, 0, 0).GetImageData());
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
//
//    glBindTexture(GL_TEXTURE_2D, 0);


    return newTexture(tex, dimensions, dim.width, dim.height, dim.depth);
}


} /* namespace gfx */
} /* namespace zephyr */
