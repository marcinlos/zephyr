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


namespace zephyr {
namespace gfx {



class Buffer {
public:

    explicit Buffer(GLuint bufferObject)
    :bufferObject_(bufferObject)
    { }


    GLuint id() const {
        return bufferObject_;
    }


    ~Buffer() {
        glDeleteBuffers(1, &bufferObject_);
    }


private:
    GLuint bufferObject_;
};

typedef std::shared_ptr<Buffer> BufferPtr;


inline BufferPtr newBuffer(GLuint bufferObject) {
    return std::make_shared<Buffer>(bufferObject);
}


struct MeshData {
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec3> normals;
    std::vector<GLuint> indices;
};


class Mesh {
public:



private:
    std::vector<BufferPtr> buffers_;
};



inline GLuint bindNewBuffer(GLenum target) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    return buffer;
}

inline GLuint bindNewVao() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    return vao;
}

inline void cleanup(GLuint vertexBuffer, GLuint indexBuffer) {
    glBindVertexArray(0);
    glDeleteBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &indexBuffer);
}

inline void* asPtr(std::size_t offset) {
    return reinterpret_cast<void*>(offset);
}

inline void setAttrib(GLuint index, std::size_t offset, int count = 4) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, count, GL_FLOAT, GL_FALSE, 0, asPtr(offset));
}


GLuint makeVertexBuffer(const std::vector<glm::vec4>& vertices) {
    std::size_t size = vertices.size() * sizeof(glm::vec4);

    GLuint vertexBuffer = bindNewBuffer(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_STATIC_DRAW);

    setAttrib(0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vertexBuffer;
}


GLuint makeIndexBuffer(const std::vector<GLuint>& indices) {
    std::size_t size = indices.size() * sizeof(GLuint);
    GLuint indexBuffer = bindNewBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, &indices[0], GL_STATIC_DRAW);
    return indexBuffer;
}


VertexArrayPtr vertexArrayFrom(const MeshData& data) {
    GLuint vao = bindNewVao();
    GLuint vertexBuffer = makeVertexBuffer(data.vertices);
    GLuint indexBuffer = makeIndexBuffer(data.indices);
    cleanup(vertexBuffer, indexBuffer);
    GLenum type = IndexTraits<GLuint>::gl_type;
    return newVertexArray(vao, data.indices.size(), true, type);
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
            a--; b--; c--;
            data.indices.push_back(a);
            data.indices.push_back(b);
            data.indices.push_back(c);
        } else if (line[0] == '#') {
            // ignore
        } else {
            // sth else, ignore
        }
    }

    data.normals = generateNormals(data.vertices, data.indices);
    return data;
}

VertexArrayPtr loadMesh(const char* filename) {
    MeshData data = loadMeshData(filename);
    return vertexArrayFrom(data);
}





}
}


#endif /* ZEPHYR_GFX_MESH_HPP_ */
