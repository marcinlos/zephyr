/**
 * @file solids.hpp
 */

#ifndef ZEPHYR_GFX_SOLIDS_HPP_
#define ZEPHYR_GFX_SOLIDS_HPP_

#include <glm/glm.hpp>
#include <cstdint>
#include <vector>

namespace zephyr {
namespace gfx {


template <typename IndexType>
std::pair<
    std::vector<glm::vec4>,
    std::vector<IndexType>
>
makeBox(float a) {
    std::vector<glm::vec4> vertices {
        a * glm::vec4 { -1, -1,  1, 1 },
        a * glm::vec4 {  1, -1,  1, 1 },
        a * glm::vec4 {  1,  1,  1, 1 },
        a * glm::vec4 { -1,  1,  1, 1 },
        a * glm::vec4 { -1, -1, -1, 1 },
        a * glm::vec4 {  1, -1, -1, 1 },
        a * glm::vec4 {  1,  1, -1, 1 },
        a * glm::vec4 { -1,  1, -1, 1 },
    };
    std::vector<IndexType> indices {
        0, 2, 1, 2, 0, 3,
        1, 6, 5, 6, 1, 2,
        5, 7, 4, 7, 5, 6,
        4, 3, 0, 3, 4, 7,
        3, 7, 6, 6, 2, 3,
        0, 1, 5, 5, 4, 0
    };
    return { move(vertices), move(indices) };
}


} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_SOLIDS_HPP_ */
