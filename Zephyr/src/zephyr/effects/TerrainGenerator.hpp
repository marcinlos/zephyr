/**
 * @file TerrainGenerator.hpp
 */

#ifndef ZEPHYR_EFFECTS_TERRAINGENERATOR_HPP_
#define ZEPHYR_EFFECTS_TERRAINGENERATOR_HPP_

#include <zephyr/gfx/objects.h>
#include <vector>


namespace zephyr {
namespace effects {

using namespace gfx;



class TerrainGenerator {
public:
    TerrainGenerator(float extent, int gridSize)
    : extent(extent)
    , onEdge(gridSize + 1)
    , vertexCount(onEdge * onEdge)
    , quadCount(gridSize * gridSize)
    , vertices(8 * vertexCount)
    , colors(&vertices[4 * vertexCount])
    , indices(3 * 2 * quadCount)
    { }

    VertexArrayPtr create() {
        generateVertices();
        generateIndices();

        modify();

        BufferGenerator gen;
        return gen(vertices, indices);
    }

    virtual ~TerrainGenerator() = default;

private:

    virtual void modify() {
        // empty
    }

    void generateVertices() {
        float fGridSize = onEdge - 1;

        float palette[][4] = {
            { 0.3f, 0.1f, 0.0f, 1 },
            { 0.1f, 0.2f, 0.2f, 1 },
            { 0.0f, 0.3f, 0.1f, 1 },
        };

        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 0; j < onEdge; ++ j) {
                int n = i * onEdge + j;
                int k = 4 * n;
                vertices[k + 0] = -extent / 2 + extent * (j / fGridSize);
                vertices[k + 1] = -2;
                vertices[k + 2] = -extent / 2 + extent * (i / fGridSize);
                vertices[k + 3] = 1;

                int r = rand();
                float* c = palette[r % std::extent<decltype(palette)>::value];
                colors[k + 0] = c[0];
                colors[k + 1] = c[1];
                colors[k + 2] = c[2];
                colors[k + 3] = c[3];
            }
        }
    }

    void generateIndices() {
        for (int i = 0, next = 0; i < onEdge - 1; ++ i) {
            for (int j = 0; j < onEdge - 1; ++ j) {
                int base = i * onEdge + j;
                indices[next ++] = base + 0;
                indices[next ++] = base + 1;
                indices[next ++] = base + onEdge + 1;
                indices[next ++] = base + onEdge + 1;
                indices[next ++] = base + onEdge;
                indices[next ++] = base + 0;
            }
        }
    }

protected:
    float extent;

    int onEdge;

    int vertexCount;

    int quadCount;

    std::vector<float> vertices;

    float* colors;

    std::vector<std::uint32_t> indices;

};



} /* namespace effects */
} /* namespace zephyr */


#endif /* ZEPHYR_EFFECTS_TERRAINGENERATOR_HPP_ */
