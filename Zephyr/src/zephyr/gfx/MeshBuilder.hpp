/**
 * @file MeshBuilder.hpp
 */

#ifndef ZEPHYR_GFX_MESHBUILDER_HPP_
#define ZEPHYR_GFX_MESHBUILDER_HPP_

#include <zephyr/gfx/objects.h>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>



namespace zephyr {
namespace gfx {


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


class MeshBuilder {
public:

    MeshBuilder()
    : vao_ { bindNewVao() }
    { }

    template <typename ItemType>
    MeshBuilder& setBuffer(const std::vector<ItemType>& data) {
        buffer_ = bindNewBuffer(GL_ARRAY_BUFFER);

        std::size_t size = data.size() * sizeof(ItemType);
        glBufferData(GL_ARRAY_BUFFER, size, &data[0], GL_STATIC_DRAW);

        return *this;
    }

    MeshBuilder& setBuffer(GLuint buffer) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        buffer_ = buffer;
        return *this;
    }

    template <typename IndexType>
    MeshBuilder& setIndices(const std::vector<IndexType>& indices) {
        indexType_ = IndexTraits<IndexType>::gl_type;
        indexCount_ = indices.size();
        indexBuffer_ = bindNewBuffer(GL_ELEMENT_ARRAY_BUFFER);

        std::size_t size = indices.size() * sizeof(IndexType);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, &indices[0], GL_STATIC_DRAW);

        return *this;
    }

    MeshBuilder& attribute(GLuint index, GLint size, std::size_t offset,
            GLenum type = GL_FLOAT,
            GLboolean normalized = false,
            GLsizei stride = 0) {
        glEnableVertexAttribArray(index);
        void* ptr = asPtr(offset);
        glVertexAttribPointer(index, size, type, normalized, stride, ptr);
        return *this;
    }

    VertexArrayPtr create() {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        if (indexed()) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &indexBuffer_);
        }
        return newVertexArray(vao_, indexCount_, indexed(), indexType_);
    }

    bool indexed() const {
        return indexCount_ != -1;
    }

private:
    GLuint bindNewVao() {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        return vao;
    }

    GLuint bindNewBuffer(GLenum target) {
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(target, buffer);
        return buffer;
    }

    void* asPtr(std::size_t offset) {
        return reinterpret_cast<void*>(offset);
    }

    GLuint vao_;
    GLuint buffer_ = -1;

    GLuint indexBuffer_;
    GLsizei indexCount_ = -1;
    GLenum indexType_;
};


} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_MESHBUILDER_HPP_ */
