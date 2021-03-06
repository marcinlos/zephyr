/**
 * @file Mesh.hpp
 */

#ifndef ZEPHYR_GFX_MESH_HPP_
#define ZEPHYR_GFX_MESH_HPP_

#include <fstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

#include <zephyr/gfx/objects.h>
#include <zephyr/gfx/MeshBuilder.hpp>

#include <iterator>
#include <random>


namespace zephyr {
namespace gfx {




struct MeshData {
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    std::vector<GLuint> indices;
};


MeshPtr vertexArrayFrom(const MeshData& data);

std::vector<glm::vec4> randomColors(std::size_t count);

/**
 * Strategy of normal vectors computation.
 */
enum class NormCalc {
    /**
     * One of the adjacent faces completely determines vertex normal
     */
    FIRST,

    /**
     * Vertex normal is computed as an average of adjacent faces' normals
     */
    AVG,

    /**
     * Vertices are duplicated, each adjacent face has its own copy of the
     * vertex, with normal equal to the face's normal
     */
    SPLIT
};


struct TangentSpace {
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
};

template <typename IndexType>
inline std::vector<glm::vec3> generateNormalsFirst(
        const std::vector<glm::vec4>& vertices,
        const std::vector<IndexType> indices) {
    std::vector<glm::vec3> normals(vertices.size());
    for (std::size_t i = 0; i < indices.size(); i += 3) {
        auto ia = indices[i];
        auto ib = indices[i + 2];
        auto ic = indices[i + 1];
        glm::vec3 ab { vertices[ib] - vertices[ia] };
        glm::vec3 ac { vertices[ic] - vertices[ia] };
        glm::vec3 n = glm::normalize(glm::cross(ab, ac));
        normals[ia] = normals[ib] = normals[ic] = n;
    }
    return normals;
}

template <typename IndexType>
TangentSpace generateTangentSpaceAvg(
        const std::vector<glm::vec4>& vertices,
        const std::vector<glm::vec2>& tex,
        const std::vector<IndexType>& indices) {
    std::size_t count = vertices.size();

    std::vector<glm::vec3> normals(count);
    std::vector<glm::vec3> tangents(count);
    std::vector<glm::vec3> bitangents(count);


    std::vector<int> times(count);
    for (std::size_t i = 0; i < indices.size(); i += 3) {
        GLuint idx[] = {
            indices[i],
            indices[i + 2],
            indices[i + 1]
        };
        glm::vec3 ab { vertices[idx[1]] - vertices[idx[0]] };
        glm::vec3 ac { vertices[idx[2]] - vertices[idx[0]] };
        glm::vec3 n = glm::cross(ab, ac);

        glm::vec2 abUV { tex[idx[1]] - tex[idx[0]] };
        glm::vec2 acUV { tex[idx[2]] - tex[idx[0]] };

        float det = abUV.x * acUV.y - abUV.y * acUV.x;
        float inv = 1.0f / det;

        glm::vec3 t = inv * (ab * acUV.y - ac * abUV.y);
        glm::vec3 b = inv * (ac * abUV.x - ab * acUV.x);

        for (int j = 0; j < 3; ++ j) {
            int k = idx[j];
            float c = 1.0f / ++ times[k];
            normals[k] += n;
            tangents[k] += t;
            bitangents[k] += b;
        }
    }
    for (auto& n : normals) {
        n = glm::normalize(n);
    }
    return TangentSpace {
        std::move(normals),
        std::move(tangents),
        std::move(bitangents)
    };
}




template <typename IndexType>
inline std::vector<glm::vec3> generateNormalsAvg(
        const std::vector<glm::vec4>& vertices,
        const std::vector<IndexType>& indices) {
    std::size_t count = vertices.size();
    std::vector<glm::vec3> normals(count);
    std::vector<int> times(count);
    for (std::size_t i = 0; i < indices.size(); i += 3) {
        GLuint idx[] = {
            indices[i],
            indices[i + 2],
            indices[i + 1]
        };
        glm::vec3 ab { vertices[idx[1]] - vertices[idx[0]] };
        glm::vec3 ac { vertices[idx[2]] - vertices[idx[0]] };
        glm::vec3 n = glm::cross(ab, ac);

        for (int j = 0; j < 3; ++ j) {
            int k = idx[j];
            float c = 1.0f / ++ times[k];
            normals[k] += n;
        }
    }
    for (auto& n : normals) {
        n = glm::normalize(n);
    }
    return normals;
}


template <typename IndexIter, typename DataIter, typename OutIter>
void duplicate(DataIter data, IndexIter begin, IndexIter end, OutIter out) {
    while (begin != end) {
        *out++ = *(data + *begin++);
    }
}


template <typename IndexType>
inline std::pair<
    std::vector<glm::vec4>,
    TangentSpace
>
generateTangentSpaceSplit(
        const std::vector<glm::vec4>& vertices,
        const std::vector<glm::vec2>& tex,
        const std::vector<IndexType>& indices) {
    std::size_t count = indices.size();
    std::vector<glm::vec4> newVertices;

    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    normals.reserve(count);
    newVertices.reserve(count);

    for (std::size_t i = 0; i < count; ++ i) {
        newVertices.push_back(vertices[indices[i]]);
        if ((i % 3) == 2) {
            auto ia = indices[i - 1];
            auto ib = indices[i - 2];
            auto ic = indices[i];
            glm::vec3 ab { vertices[ib] - vertices[ia] };
            glm::vec3 ac { vertices[ic] - vertices[ia] };

            glm::vec2 abUV { tex[ib] - tex[ia] };
            glm::vec2 acUV { tex[ic] - tex[ia] };

            float det = abUV.x * acUV.y - abUV.y * acUV.x;
            float inv = 1.0f / det;

            glm::vec3 t = inv * (ab * acUV.y - ac * abUV.y);
            glm::vec3 b = inv * (ac * abUV.x - ab * acUV.x);

            glm::vec3 n = glm::normalize(glm::cross(ab, ac));
            for (int j = 0; j < 3; ++ j) {
                normals.push_back(n);
                tangents.push_back(t);
                bitangents.push_back(b);
            }
        }
    }

    return {
        std::move(newVertices),
        TangentSpace {
            std::move(normals),
            std::move(tangents),
            std::move(bitangents)
        }
    };
}


template <typename IndexType>
inline std::pair<
    std::vector<glm::vec4>,
    std::vector<glm::vec3>
>
generateNormalsSplit(
        const std::vector<glm::vec4>& vertices,
        const std::vector<IndexType>& indices) {
    std::size_t count = indices.size();
    std::vector<glm::vec3> normals;
    std::vector<glm::vec4> newVertices;

    normals.reserve(count);
    newVertices.reserve(count);

    for (std::size_t i = 0; i < count; ++ i) {
        newVertices.push_back(vertices[indices[i]]);
        if ((i % 3) == 2) {
            auto ia = indices[i - 1];
            auto ib = indices[i - 2];
            auto ic = indices[i];
            glm::vec3 ab { vertices[ib] - vertices[ia] };
            glm::vec3 ac { vertices[ic] - vertices[ia] };
            glm::vec3 n = glm::normalize(glm::cross(ab, ac));
            for (int j = 0; j < 3; ++ j) {
                normals.push_back(n);
            }
        }
    }

    return std::make_pair(std::move(newVertices), std::move(normals));
}

MeshData loadObjData(const char* path, NormCalc strategy = NormCalc::AVG);

MeshPtr loadObjMesh(const char* path, NormCalc strategy = NormCalc::AVG);

} /* namespace gfx */
} /* namespace zephyr */


#endif /* ZEPHYR_GFX_MESH_HPP_ */
