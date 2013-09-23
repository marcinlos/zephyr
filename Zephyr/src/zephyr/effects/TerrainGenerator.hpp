/**
 * @file TerrainGenerator.hpp
 */

#ifndef ZEPHYR_EFFECTS_TERRAINGENERATOR_HPP_
#define ZEPHYR_EFFECTS_TERRAINGENERATOR_HPP_

#include <zephyr/gfx/objects.h>
#include <zephyr/gfx/MeshBuilder.hpp>
#include <vector>

#include <zephyr/gfx/Mesh.hpp>
#include <zephyr/effects/Grid.hpp>


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
    , indices(3 * 2 * quadCount)
    , vertices(vertexCount)
    , colors(vertexCount, glm::vec4 { 1, 1, 1, 1 })
    , texture(vertexCount)
    { }

    MeshPtr create() {
        generateVertices();
        generateIndices();

        modify();

        MeshData data;
        TangentSpace tg;
        auto pair = std::tie(data.vertices, tg);
        pair = generateTangentSpaceSplit(vertices, texture, indices);

        std::size_t count = data.vertices.size();
        data.uv.reserve(count);
        data.normals.reserve(count);
        data.tangents.reserve(count);
        data.bitangents.reserve(count);

        auto from = begin(indices), to = end(indices);
        duplicate(begin(texture), from, to, back_inserter(data.uv));
//            duplicate(begin(tg.normals), from, to, back_inserter(data.normals));
//            duplicate(begin(tg.tangents), from, to, back_inserter(data.tangents));
//            duplicate(begin(tg.bitangents), from, to, back_inserter(data.bitangents));
        data.normals = std::move(tg.normals);
        data.tangents = std::move(tg.tangents);
        data.bitangents = std::move(tg.bitangents);

        return vertexArrayFrom(data);
        /*
        //auto normals = generateNormalsAvg(vertices, indices);
        auto data = generateTangentSpaceSplit(vertices, texture, indices);
        duplicate(begin(texture), , back_inserter(data.uv));
        return MeshBuilder()
                .setBuffer(data.first).attribute(0, 4)
                .setBuffer(colors).attribute(1, 4)
                .setBuffer(data.second.normals).attribute(2, 3)
                .setBuffer(texture).attribute(3, 2)
                .setBuffer(data.second.tangents).attribute(4, 3)
                .setBuffer(data.second.bitangents).attribute(5, 3)
                .setIndices(indices)
                .create();*/
    }

    virtual ~TerrainGenerator() = default;

    void fromHeights(const std::vector<float>& height) {
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 0; j < onEdge; ++ j) {
                int n = i * onEdge + j;
                vertices[n].y = height[n];
            }
        }
    }

private:

    virtual void modify() {
        // empty
    }

    float lerp(int i, float min, float max) {
        float a = i / float(onEdge - 1);
        return min + a * (max - min);
    }

    float lerp(int i) {
        return lerp(i, -extent/2, extent/2);
    }

    void generateVertices() {
        float fGridSize = onEdge - 1;

        float palette[][4] = {
            { 0.2f, 0.8f, 0.27f, 1 },
            { 0.2f, 0.7f, 0.15f, 1 },
            { 0.15f, 0.95f, 0.15f, 1 },
        };

        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 0; j < onEdge; ++ j) {
                int n = i * onEdge + j;

                vertices[n] = glm::vec4 { lerp(j), 0, lerp(i), 1 };
                texture[n] = glm::vec2 { lerp(j, 0, 1), lerp(i, 0, 1) };

                int r = rand();
                float* c = palette[r % std::extent<decltype(palette)>::value];
                colors[i * onEdge + j] = glm::vec4 { c[0], c[1], c[2], c[3] };
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
                indices[next ++] = base + onEdge;
                indices[next ++] = base + 0;
                indices[next ++] = base + onEdge + 1;
            }
        }
    }

protected:
    float extent;

    int onEdge;

    int vertexCount;

    int quadCount;

    std::vector<std::uint32_t> indices;

    std::vector<glm::vec4> vertices;

    std::vector<glm::vec4> colors;

    std::vector<glm::vec2> texture;
};



} /* namespace effects */
} /* namespace zephyr */


#endif /* ZEPHYR_EFFECTS_TERRAINGENERATOR_HPP_ */
