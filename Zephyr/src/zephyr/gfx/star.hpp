/**
 * @file star.hpp
 */

#ifndef ZEPHYR_GFX_STAR_HPP_
#define ZEPHYR_GFX_STAR_HPP_

#include <zephyr/gfx/Mesh.hpp>
#include <glm/glm.hpp>
#include <cstdlib>
#include <cmath>
#include <vector>


namespace zephyr {
namespace gfx {


inline std::vector<glm::vec4> makeStarVertices(int n, float w) {
    std::vector<glm::vec4> v(2 * n + 2);
    const float d = M_PI / n;

    for (int i = 0; i < 2 * n; ++ i) {
        float theta = i * M_PI / n;

        float x = std::sin(theta);
        float y = std::cos(theta);

        float s = (i % 2 == 0) ? 1.0f : 0.5f;

        v[i] = glm::vec4 { s * x, s * y, 0, 1 };
    }
    v[2 * n + 0] = glm::vec4 { 0, 0,  w, 1 };
    v[2 * n + 1] = glm::vec4 { 0, 0, -w, 1 };
    return v;
}

template <typename IndexType>
inline std::vector<IndexType> makeStarIndices(int n) {
    std::vector<IndexType> idx(2 * 2 * n * 3);
    for (int j = 2 * n - 1, i = 0; i < 2 * n; j = i++) {
        int k = 2 * 3 * i, m = k + 3;
        idx[k + 0] = 2 * n;
        idx[m + 0] = 2 * n + 1;
        idx[k + 1] = idx[m + 2] = j;
        idx[k + 2] = idx[m + 1] = i;
    }
    return idx;
}

inline MeshPtr makeStar(int n, float w) {
    auto vertices = makeStarVertices(n, w);
    auto indices = makeStarIndices<GLuint>(n);

    std::vector<glm::vec4> v2;
    std::vector<glm::vec3> i2;

    std::tie(v2, i2) = generateNormalsSplit(vertices, indices);

    return MeshBuilder()
            .setBuffer(v2).attribute(0, 4)
            .setBuffer(randomColors(12 * n)).attribute(1, 4)
            .setBuffer(i2).attribute(2, 3)
//            .setIndices(indices)
            .create();
}

} /* namespace gfx */
} /* namespace zephyr */


#endif /* ZEPHYR_GFX_STAR_HPP_ */
