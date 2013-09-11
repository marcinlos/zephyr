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

#include <random>


namespace zephyr {
namespace gfx {



//class Buffer {
//public:
//
//    explicit Buffer(GLuint bufferObject)
//    :bufferObject_(bufferObject)
//    { }
//
//
//    GLuint id() const {
//        return bufferObject_;
//    }
//
//
//    ~Buffer() {
//        glDeleteBuffers(1, &bufferObject_);
//    }
//
//
//private:
//    GLuint bufferObject_;
//};
//
//typedef std::shared_ptr<Buffer> BufferPtr;
//
//
//inline BufferPtr newBuffer(GLuint bufferObject) {
//    return std::make_shared<Buffer>(bufferObject);
//}


struct MeshData {
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec3> normals;
    std::vector<GLuint> indices;
};


/*
template <typename IndexType>
struct IndexTraits;

template <>
struct IndexTraits<std::uint8_t> {
    enum { gl_type = GL_UNSIGNED_BYTE };
};

template <>
struct IndexTraits<std::uint16_t> {
    enum { gl_type = GL_UNSIGNED_SHORT };
};

template <>
struct IndexTraits<std::uint32_t> {
    enum { gl_type = GL_UNSIGNED_INT };
};
*/


VertexArrayPtr vertexArrayFrom(const MeshData& data) {
    MeshBuilder builder;
    return builder
            .setBuffer(data.vertices).attribute(0, 4, 0)
            .setBuffer(data.colors).attribute(1, 4, 0)
            .setBuffer(data.normals).attribute(2, 3, 0)
            .setIndices(data.indices)
            .create();
}


std::vector<glm::vec3> generateNormals(const std::vector<glm::vec4>& vertices,
        const std::vector<GLuint> indices) {
    std::vector<glm::vec3> normals(vertices.size());
    for (std::size_t i = 0; i < indices.size(); i += 3) {
        auto ia = indices[i];
        auto ib = indices[i + 1];
        auto ic = indices[i + 2];
        glm::vec3 ab { vertices[ib] - vertices[ia] };
        glm::vec3 ac { vertices[ic] - vertices[ia] };
        glm::vec3 normal = glm::normalize(glm::cross(ab, ac));
        normals[ia] = normals[ib] = normals[ic] = normal;
    }
    return normals;
}




std::vector<glm::vec4> generateRandomColors(std::size_t count) {
    std::default_random_engine generator(std::time(nullptr));
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    std::vector<glm::vec4> colors(count * 4);
    for (auto& color : colors) {
        color.x = distribution(generator);
        color.y = distribution(generator);
        color.z = distribution(generator);
        color.w = 1.0f;
    }
    return colors;
}


MeshData loadMeshData(const char* filename) {
    std::ifstream in(filename, std::ios::in);
    if (!in) {
        std::ostringstream ss("Cannot open file: ");
        ss << filename;
        throw new std::runtime_error(ss.str());
    }

    MeshData data;

    std::string line;
    while (std::getline(in, line)) {
        if (line.substr(0, 2) == "v ") {
            std::istringstream s(line.substr(2));
            glm::vec4 v;
            s >> v.x; s >> v.y; s >> v.z;
            v.w = 1.0f;
            data.vertices.push_back(v);
        } else if (line.substr(0, 2) == "f ") {
            std::istringstream s(line.substr(2));
            GLushort a, b, c;
            s >> a; s >> b; s >> c;
            // inversion!
            data.indices.push_back(a - 1);
            data.indices.push_back(c - 1);
            data.indices.push_back(b - 1);
        } else {
            // sth else, ignore
        }
    }

    data.normals = generateNormals(data.vertices, data.indices);
    data.colors = generateRandomColors(data.vertices.size());
    return data;
}

VertexArrayPtr loadMesh(const char* filename) {
    MeshData data = loadMeshData(filename);
    return vertexArrayFrom(data);
}





}
}


#endif /* ZEPHYR_GFX_MESH_HPP_ */
