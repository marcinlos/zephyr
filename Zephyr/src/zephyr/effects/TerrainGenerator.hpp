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


struct PointProxy {
    float& x;
    float& y;
    float& z;
    float& w;

    PointProxy(float* data)
    : x(data[0])
    , y(data[1])
    , z(data[2])
    , w(data[3])
    { }
};


struct RowProxy {
    float* vertices;
    int row;
    int inRow;


    RowProxy(float* vertices, int row, int inRow)
    : vertices(vertices)
    , row(row)
    , inRow(inRow)
    { }

    PointProxy operator [] (int col) {
        int offset = row * inRow + col;
        return PointProxy { &vertices[offset << 2] };
    }
};

struct Grid {
    float* vertices;
    int inRow;

    Grid(float* vertices, int inRow)
    : vertices(vertices)
    , inRow(inRow)
    { }

    RowProxy operator [] (int row) {
        return RowProxy { vertices, row, inRow };
    }
};



class TerrainGenerator {
public:
    TerrainGenerator(float extent, int gridSize)
    : extent(extent)
    , onEdge(gridSize + 1)
    , vertexCount(onEdge * onEdge)
    , quadCount(gridSize * gridSize)
    , data(8 * vertexCount)
    , indices(3 * 2 * quadCount)
    , colors(&data[0] + 4 * vertexCount, onEdge)
    , v(&data[0], onEdge)
    { }

    VertexArrayPtr create() {
        generateVertices();
        generateIndices();

        modify();

        BufferGenerator gen;
        return gen(data, indices);
    }

    virtual ~TerrainGenerator() = default;

private:

    virtual void modify() {
        // empty
    }

    void generateVertices() {
        float fGridSize = onEdge - 1;

        float palette[][4] = {
            { 0.2f, 0.1f, 0.07f, 1 },
            { 0.2f, 0.2f, 0.1f, 1 },
            { 0.25f, 0.15f, 0.05f, 1 },
        };

        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 0; j < onEdge; ++ j) {
                int n = i * onEdge + j;
                int k = 4 * n;
                v[i][j].x = -extent / 2 + extent * (j / fGridSize);
                v[i][j].y = 0;
                v[i][j].z = -extent / 2 + extent * (i / fGridSize);
                v[i][j].w = 1;

                int r = rand();
                float* c = palette[r % std::extent<decltype(palette)>::value];
                colors[i][j].x = c[0];
                colors[i][j].y = c[1];
                colors[i][j].z = c[2];
                colors[i][j].w = c[3];
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


    std::vector<std::uint32_t> indices;

    Grid v;

    Grid colors;

private:
    std::vector<float> data;
};



} /* namespace effects */
} /* namespace zephyr */


#endif /* ZEPHYR_EFFECTS_TERRAINGENERATOR_HPP_ */
