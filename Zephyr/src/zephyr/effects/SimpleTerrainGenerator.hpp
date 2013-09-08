/**
 * @file SimpleTerrainGenerator.hpp
 */

#ifndef ZEPHYR_EFFECTS_SIMPLETERRAINGENERATOR_HPP_
#define ZEPHYR_EFFECTS_SIMPLETERRAINGENERATOR_HPP_

#include <zephyr/effects/TerrainGenerator.hpp>

namespace zephyr {
namespace effects {

class SimpleTerrainGenerator : public TerrainGenerator {
public:

    SimpleTerrainGenerator(float extent, int gridSize)
    : TerrainGenerator(extent, gridSize)
    { }

private:
    void modify() override {
        for (int iters = 0; iters < 100; ++ iters) {
            for (int i = 0; i < onEdge; ++ i) {
                for (int j = 0; j < onEdge; ++ j) {

                }
            }
        }
    }

};


} /* namespace effects */
} /* namespace zephyr */


#endif /* ZEPHYR_EFFECTS_SIMPLETERRAINGENERATOR_HPP_ */
