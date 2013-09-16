/**
 * @file UniformManager.hpp
 */

#ifndef ZEPHYR_GFX_UNIFORMMANAGER_HPP_
#define ZEPHYR_GFX_UNIFORMMANAGER_HPP_

#include <zephyr/gfx/uniforms.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <memory>


namespace zephyr {
namespace gfx {

class UniformManager {
public:

    void set(std::string name, UniformPtr uniform);

    Uniform* get(const std::string& name) {
        auto it = uniforms_.find(name);
        if (it != end(uniforms_)) {
            return it->second.get();
        } else {
            return nullptr;
        }
    }

    GLuint registerBlock(const std::string& name, GLuint buffer,
            std::size_t size) {
        GLuint index = nextBindingIndex_ ++;
        blocks_[name] = index;
        buffers_[name] = buffer;
        glBindBufferRange(GL_UNIFORM_BUFFER, index, buffer, 0, size);
        return index;
    }

    GLuint createBlock(const std::string& name,  GLsizei size) {
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STREAM_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        return registerBlock(name, buffer, size);
    }

    GLint blockBindingIndex(const std::string& name) {
        auto it = blocks_.find(name);
        if (it != end(blocks_)) {
            return it->second;
        } else {
            return -1;
        }
    }

    void fillBlock(const std::string& name, void* data, std::ptrdiff_t offset,
            std::size_t size) {
        GLuint buffer = buffers_[name];
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void set1f(std::string name, float v) {
        set(std::move(name), std::make_shared<uniform1f>(v));
    }

    void set1f(std::string name, glm::vec1 v) {
        set1f(std::move(name), v.x);
    }

    void set2f(std::string name, float v1, float v2) {
        set(std::move(name), std::make_shared<uniform2f>(v1, v2));
    }

    void set2f(std::string name, const glm::vec2& v) {
        set2f(std::move(name), v.x, v.y);
    }

    void set3f(std::string name, float v1, float v2, float v3) {
        set(std::move(name), std::make_shared<uniform3f>(v1, v2, v3));
    }

    void set3f(std::string name, const glm::vec3& v) {
        set3f(std::move(name), v.x, v.y, v.z);
    }

    void set4f(std::string name, float v1, float v2, float v3, float v4) {
        set(std::move(name), std::make_shared<uniform4f>(v1, v2, v3, v4));
    }

    void set4f(std::string name, const glm::vec4& v) {
        set4f(std::move(name), v.x, v.y, v.z, v.w);
    }

    void set1i(std::string name, GLint v) {
        set(std::move(name), std::make_shared<uniform1i>(v));
    }

    void set1i(std::string name, GLint v1, GLint v2) {
        set(std::move(name), std::make_shared<uniform2i>(v1, v2));
    }

    void set1i(std::string name, GLint v1, GLint v2, GLint v3) {
        set(std::move(name), std::make_shared<uniform3i>(v1, v2, v3));
    }

    void set1i(std::string name, GLint v1, GLint v2, GLint v3, GLint v4) {
        set(std::move(name), std::make_shared<uniform4i>(v1, v2, v3, v4));
    }

    void set1ui(std::string name, GLuint v) {
        set(std::move(name), std::make_shared<uniform1i>(v));
    }

    void set1ui(std::string name, GLuint v1, GLuint v2) {
        set(std::move(name), std::make_shared<uniform2i>(v1, v2));
    }

    void set1ui(std::string name, GLuint v1, GLuint v2, GLuint v3) {
        set(std::move(name), std::make_shared<uniform3i>(v1, v2, v3));
    }

    void set1ui(std::string name, GLuint v1, GLuint v2, GLuint v3, GLuint v4) {
        set(std::move(name), std::make_shared<uniform4i>(v1, v2, v3, v4));
    }

    void setMat2(std::string name, const glm::mat2& mat,
            bool transpose = false, int count = 1) {
        set(std::move(name),
                std::make_shared<uniformMatrix2f>(mat, count, transpose));
    }

    void setMat3(std::string name, const glm::mat3& mat,
            bool transpose = false, int count = 1) {
        set(std::move(name),
                std::make_shared<uniformMatrix3f>(mat, count, transpose));
    }

    void setMat4(std::string name, const glm::mat4& mat,
            bool transpose = false, int count = 1) {
        set(std::move(name),
                std::make_shared<uniformMatrix4f>(mat, count, transpose));
    }

    void setMat2x3(std::string name, const glm::mat2x3& mat,
            bool transpose = false, int count = 1) {
        set(std::move(name),
                std::make_shared<uniformMatrix2x3f>(mat, count, transpose));
    }

    void setMat3x2(std::string name, const glm::mat3x2& mat,
            bool transpose = false, int count = 1) {
        set(std::move(name),
                std::make_shared<uniformMatrix3x2f>(mat, count, transpose));
    }

    void setMat2x4(std::string name, const glm::mat2x4& mat,
            bool transpose = false, int count = 1) {
        set(std::move(name),
                std::make_shared<uniformMatrix2x4f>(mat, count, transpose));
    }

    void setMat4x2(std::string name, const glm::mat4x2& mat,
            bool transpose = false, int count = 1) {
        set(std::move(name),
                std::make_shared<uniformMatrix4x2f>(mat, count, transpose));
    }

    void setMat4x3(std::string name, const glm::mat4x3& mat,
            bool transpose = false, int count = 1) {
        set(std::move(name),
                std::make_shared<uniformMatrix4x3f>(mat, count, transpose));
    }

    void setMat3x4(std::string name, const glm::mat3x4& mat,
            bool transpose = false, int count = 1) {
        set(std::move(name),
                std::make_shared<uniformMatrix3x4f>(mat, count, transpose));
    }

private:
    typedef std::unordered_map<std::string, UniformPtr> UniformMap;

    UniformMap uniforms_;

    std::unordered_map<std::string, GLuint> blocks_;

    std::unordered_map<std::string, GLuint> buffers_;

    GLint nextBindingIndex_ = 1;

};

inline void UniformManager::set(std::string name, UniformPtr uniform) {
    using std::move;
    uniforms_.insert(std::make_pair(move(name), move(uniform)));
}

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_UNIFORMMANAGER_HPP_ */
