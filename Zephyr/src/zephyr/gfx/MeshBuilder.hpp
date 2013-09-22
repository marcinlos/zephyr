/**
 * @file MeshBuilder.hpp
 */

#ifndef ZEPHYR_GFX_MESHBUILDER_HPP_
#define ZEPHYR_GFX_MESHBUILDER_HPP_

#include <zephyr/gfx/objects.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <algorithm>


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


struct AttributeData {
    GLuint index;
    GLint size;
    std::size_t offset;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;

    AttributeData(GLuint index, GLint size,
        std::size_t offset = 0,
        GLenum type = GL_FLOAT,
        GLboolean normalized = false,
        GLsizei stride = 0)
    : index { index }
    , size { size }
    , offset { offset }
    , type { type }
    , normalized { normalized }
    , stride { stride }
    { }
};


class MeshBuilder {
public:

    MeshBuilder()
    : vao_ { bindNewVao() }
    { }

    template <typename ItemType>
    MeshBuilder& setBuffer(const std::vector<ItemType>& data) {
        updateMinSize(data.size());
        vertexBuffer_ = bindNewBuffer(GL_ARRAY_BUFFER);

        std::size_t size = data.size() * sizeof(ItemType);
        glBufferData(GL_ARRAY_BUFFER, size, &data[0], GL_STATIC_DRAW);

        return *this;
    }

    template <typename ItemType, std::size_t N>
    MeshBuilder& setBuffer(const ItemType (&data)[N]) {
        updateMinSize(N);
        vertexBuffer_ = bindNewBuffer(GL_ARRAY_BUFFER);

        std::size_t size = sizeof(data);
        glBufferData(GL_ARRAY_BUFFER, size, &data[0], GL_STATIC_DRAW);

        return *this;
    }

    MeshBuilder& setBuffer(GLuint buffer) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        vertexBuffer_ = buffer;
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

    MeshBuilder& attribute(GLuint index, GLint size,
            std::size_t offset = 0,
            GLenum type = GL_FLOAT,
            GLboolean normalized = false,
            GLsizei stride = 0) {
        glEnableVertexAttribArray(index);
        void* ptr = asPtr(offset);
        glVertexAttribPointer(index, size, type, normalized, stride, ptr);
        return *this;
    }

    MeshBuilder& attribute(const AttributeData& attr) {
        glEnableVertexAttribArray(attr.index);
        void* ptr = asPtr(attr.offset);
        glVertexAttribPointer(attr.index, attr.size, attr.type,
                attr.normalized, attr.stride, ptr);
        return *this;
    }

    MeshPtr create(Primitive mode = Primitive::TRIANGLES) {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        if (indexed()) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &indexBuffer_);
        }
        std::cout << "Creating with " << vertexCount() << " items!" << std::endl;
        return newMesh(vao_, vertexCount(), indexed(), indexType_, mode);
    }

    bool indexed() const {
        return indexBuffer_ != static_cast<GLuint>(-1);
    }

    bool empty() const {
        return vertexBuffer_ == static_cast<GLuint>(-1);
    }

    GLsizei vertexCount() const {
        return indexed() ? indexCount_
                : empty() ? 0 : vertexCount_;
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

    void updateMinSize(GLsizei size) {
        if (empty() || vertexCount_ > size) {
            vertexCount_ = size;
        }
    }

    GLuint vao_;
    GLuint vertexBuffer_ = -1;
    GLsizei vertexCount_;

    GLuint indexBuffer_ = -1;
    GLsizei indexCount_;
    GLenum indexType_;
};


} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_MESHBUILDER_HPP_ */
