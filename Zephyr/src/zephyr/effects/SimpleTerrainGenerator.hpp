/**
 * @file SimpleTerrainGenerator.hpp
 */

#ifndef ZEPHYR_EFFECTS_SIMPLETERRAINGENERATOR_HPP_
#define ZEPHYR_EFFECTS_SIMPLETERRAINGENERATOR_HPP_

#include <zephyr/effects/TerrainGenerator.hpp>
#include <zephyr/effects/DiamondSquareNoise.hpp>
#include <zephyr/effects/NoiseSmoother.hpp>
#include <cstdlib>
#include <queue>

namespace zephyr {
namespace effects {

class SimpleTerrainGenerator : public TerrainGenerator {
public:

    SimpleTerrainGenerator(float extent, int iterations, float power)
    : TerrainGenerator(extent, 1 << iterations)
    , iterations(iterations)
    , power(power)
    { }

private:
    void modify() override {
        std::vector<float> height = diamond::noise({ iterations, power });

        auto grid = make_grid(height, onEdge);
        simpleSmooth(grid, onEdge, 0.9f, 4);

        fromHeights(height);
//        makeFlat();
        auto colorGrid = make_grid(colors, onEdge);
        simpleSmooth(colorGrid, onEdge, 0.7f, 3);
        colorFromHeight(0.3f);
    }


    void makeFlat() {
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 0; j < onEdge; ++ j) {
                vertices[i * onEdge + j].y = 0.0f;
            }
        }
    }
    void colorFromHeight(float power) {
        float min = 1000.0f, max = -1000.0f;
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 0; j < onEdge; ++ j) {
                float x = vertices[i * onEdge + j].y;
                min = std::min(min, x);
                max = std::max(max, x);
            }
        }
        std::cout << "[" << min << ", " << max << "]\n";
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 0; j < onEdge; ++ j) {
                float x = vertices[i * onEdge + j].y;
                float color = (x - min) / (max - min);
                float mult = 1 - power + power * color;
                colors[i * onEdge + j] *= mult;
                colors[i * onEdge + j].w = 1.0f;
            }
        }
    }

    float random() const {
        return 2 * (rand() / (0.0f + RAND_MAX) - 0.5f);
    }

    int iterations;

    float power;
};


} /* namespace effects */
} /* namespace zephyr */


#endif /* ZEPHYR_EFFECTS_SIMPLETERRAINGENERATOR_HPP_ */
