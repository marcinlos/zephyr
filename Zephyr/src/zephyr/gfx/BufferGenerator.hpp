/**
 * @file BufferGenerator.hpp
 */

#ifndef ZEPHYR_GFX_BUFFERGENERATOR_HPP_
#define ZEPHYR_GFX_BUFFERGENERATOR_HPP_

#include <zephyr/gfx/objects.h>
#include <cstdint>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

namespace zephyr {
namespace gfx {



struct BufferGenerator {

    static const std::size_t posSize = 4 * sizeof(float);
    static const std::size_t colorSize = 4 * sizeof(float);
    static const std::size_t vertexSize = posSize + colorSize;
    static const std::size_t indexSize = sizeof(std::uint16_t);

    VertexArrayPtr operator () (
            const std::vector<float>& vertices,
            const std::vector<std::uint16_t>& indices
    ) const {
        GLuint vao = bindNewVao();
        GLuint vertexBuffer = makeVertexBuffer(vertices);
        GLuint indexBuffer = makeIndexBuffer(indices);

        cleanup(vertexBuffer, indexBuffer);
        return newVertexArray(vao, indices.size(), true);
    }

private:
    GLuint makeVertexBuffer(const std::vector<float>& vertices) const {
        std::size_t size = vertices.size() * sizeof(float);
        std::size_t vertexCount = size / vertexSize;
        void* colorOffset = asPtr(vertexCount * posSize);

        GLuint vertexBuffer = bindNewBuffer(GL_ARRAY_BUFFER);
        glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_STATIC_DRAW);

        attrib(0, 0);
        attrib(1, vertexCount * posSize);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return vertexBuffer;
    }

    GLuint makeIndexBuffer(const std::vector<std::uint16_t>& indices) const {
        std::size_t size = indices.size() * sizeof(std::uint16_t);
        GLuint indexBuffer = bindNewBuffer(GL_ELEMENT_ARRAY_BUFFER);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, &indices[0], GL_STATIC_DRAW);
        return indexBuffer;
    }

    GLuint bindNewBuffer(GLenum target) const {
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(target, buffer);
        return buffer;
    }

    GLuint bindNewVao() const {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        return vao;
    }

    void cleanup(GLuint vertexBuffer, GLuint indexBuffer) const {
        glBindVertexArray(0);
        glDeleteBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &indexBuffer);
    }

    void attrib(GLuint index, std::size_t offset, int count = 4) const {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, count, GL_FLOAT, GL_FALSE, 0, asPtr(offset));
    }

    void* asPtr(std::size_t offset) const {
        return reinterpret_cast<void*>(offset);
    }

};



} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_BUFFERGENERATOR_HPP_ */
