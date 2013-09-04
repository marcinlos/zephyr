/*
 * star.hpp
 *
 *  Created on: Sep 4, 2013
 *      Author: los
 */

#ifndef ZEPHYR_GFX_STAR_HPP_
#define ZEPHYR_GFX_STAR_HPP_

#include <cstdlib>
#include <cmath>
#include <vector>


namespace zephyr {
namespace gfx {

inline float randFloat() {
    return (1.0f * std::rand()) / RAND_MAX;
}

inline std::vector<float> makeStar(int n, float w) {
    std::vector<float> storage(2 * 4 * 3 * 4 * n);
    float* v = &storage[0];
    float* col = v +  4 * 3 * 4 *n;

    const float d = M_PI / n;

    for (int i = 0; i < 4 * 3 * n; ++i) {
        col[i * 4 + 0] = randFloat();
        col[i * 4 + 1] = randFloat();
        col[i * 4 + 2] = randFloat();
        col[i * 4 + 3] = 1.0f;
    }

    for (int i = 0; i < n; ++ i) {
        float theta = 2 * i * M_PI / n;
        float x = std::sin(theta);
        float y = std::cos(theta);

        float s = 0.5f;
        float px = s * std::sin(theta - d);
        float py = s * std::cos(theta - d);

        float nx = s * std::sin(theta + d);
        float ny = s * std::cos(theta + d);

        int k = i * 4 * 3 * 4;

        v[k + 0] = 0.0f;
        v[k + 1] = 0.0f;
        v[k + 2] = w;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = px;
        v[k + 1] = py;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = x;
        v[k + 1] = y;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;


        k += 4;
        v[k + 0] = 0.0f;
        v[k + 1] = 0.0f;
        v[k + 2] = -w;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = x;
        v[k + 1] = y;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = px;
        v[k + 1] = py;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;



        k += 4;
        v[k + 0] = 0.0f;
        v[k + 1] = 0.0f;
        v[k + 2] = w;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = x;
        v[k + 1] = y;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = nx;
        v[k + 1] = ny;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;


        k += 4;
        v[k + 0] = 0.0f;
        v[k + 1] = 0.0f;
        v[k + 2] = -w;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = nx;
        v[k + 1] = ny;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = x;
        v[k + 1] = y;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;

    }

    return storage;
}

} /* namespace gfx */
} /* namespace zephyr */


#endif /* ZEPHYR_GFX_STAR_HPP_ */
