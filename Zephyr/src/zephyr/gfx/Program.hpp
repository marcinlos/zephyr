/**
 * @file Program.hpp
 */

#ifndef ZEPHYR_GFX_PROGRAM_HPP_
#define ZEPHYR_GFX_PROGRAM_HPP_

#include <zephyr/gfx/Shader.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <memory>
#include <unordered_map>


namespace zephyr {
namespace gfx {

namespace detail {

    void checkCreationStatus(GLuint program) {
        int status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status != GL_TRUE) {
            GLint infoLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
            std::vector<GLchar>  buffer(infoLength + 1);
            glGetProgramInfoLog(program, infoLength, nullptr, buffer.data());

            throw std::runtime_error(util::format(
                    "Error while linking program: {}", buffer.data()));
        }
    }

    template <typename Iter>
    GLuint create(Iter begin, Iter end) {
        GLuint program = glCreateProgram();
        for (Iter s = begin; s != end; ++ s) {
            glAttachShader(program, (*s)->ref());
        }
        glLinkProgram(program);

        checkCreationStatus(program);
        for (Iter s = begin; s != end; ++ s) {
            glDetachShader(program, (*s)->ref());
        }
        return program;
    }

    std::unordered_map<std::string, GLuint> getUniforms(GLuint program) {
        std::unordered_map<std::string, GLuint> uniforms;

        GLint uniformCount;
        glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformCount);
        for (int i = 0; i < uniformCount; ++ i) {
            const std::size_t maxLength = 256;
            char buffer[maxLength + 1];
            GLint size;
            GLenum type;
            glGetActiveUniform(program, i, maxLength, nullptr, &size, &type,
                    buffer);
            buffer[maxLength] = '\0';
            uniforms[buffer] = glGetUniformLocation(program, buffer);
        }
        return uniforms;
    }

    std::unordered_map<std::string, GLuint> getUniformBlocks(GLuint program) {
        std::unordered_map<std::string, GLuint> blocks;

        GLint blockCount;
        glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &blockCount);
        for (int i = 0; i < blockCount; ++ i) {
            const std::size_t maxLength = 256;
            char buffer[maxLength + 1];
            glGetActiveUniformBlockName(program, i, maxLength, nullptr, buffer);
            blocks[buffer] = glGetUniformBlockIndex(program, buffer);
        }

        return blocks;
    }

} /* namespace detail */


class Program: public std::enable_shared_from_this<Program> {
public:

    template <typename Iter>
    Program (Iter begin, Iter end)
    : program_ { detail::create(begin, end) }
    , uniforms_ { detail::getUniforms(program_) }
    , uniformBlocks_ { detail::getUniformBlocks(program_) }
    { }

    template <typename Container>
    Program (const Container& shaders)
    : Program(begin(shaders), end(shaders))
    { }

    Program(std::initializer_list<ShaderPtr> shaders)
    : Program(begin(shaders), end(shaders))
    { }

    ~Program() {
        glDeleteProgram(program_);
    }

    GLuint ref() const {
        return program_;
    }

    GLint uniformLocation(const char* name) const {
        GLint location = glGetUniformLocation(program_, name);
        if (location != -1) {
            return location;
        } else {
            throw std::runtime_error(util::format(
                    "Cannot get location of \"{}\" uniform variable", name));
        }
    }

    GLint uniformLocation(const std::string& name) const {
        auto it = uniforms_.find(name);
        if (it != uniforms_.end()) {
            return it->second;
        } else {
            throw std::runtime_error(util::format(
                    "Cannot get location of \"{}\" uniform variable", name));
        }
    }

    GLuint uniformBlockIndex(const char* name) const {
        return uniformBlockIndex(std::string { name });
    }

    GLuint uniformBlockIndex(const std::string& name) const {
        auto it = uniformBlocks_.find(name);
        if (it != uniforms_.end()) {
            return it->second;
        } else {
            throw std::runtime_error(util::format(
                    "Cannot get location of \"{}\" uniform block", name));
        }
    }

    void bindBlock(const std::string& name, GLuint bindingIndex) {
        GLuint index = uniformBlockIndex(name);
        bindBlock(index, bindingIndex);
    }

    void bindBlock(GLuint index, GLuint bindingIndex) {
        glUniformBlockBinding(program_, index, bindingIndex);
    }

private:

    GLuint program_;

    std::unordered_map<std::string, GLuint> uniforms_;

    std::unordered_map<std::string, GLuint> uniformBlocks_;

};

typedef std::shared_ptr<Program> ProgramPtr;

ProgramPtr newProgram(std::initializer_list<ShaderPtr> shaders) {
    return std::make_shared<Program>(shaders);
}


} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_PROGRAM_HPP_ */
