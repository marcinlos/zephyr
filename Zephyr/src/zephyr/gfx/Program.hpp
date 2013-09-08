/**
 * @file Program.hpp
 */

#ifndef ZEPHYR_GFX_PROGRAM_HPP_
#define ZEPHYR_GFX_PROGRAM_HPP_

#include <zephyr/gfx/Shader.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <memory>


namespace zephyr {
namespace gfx {



void checkCreationStatus(GLuint program) {
    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        GLint infoLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
        std::vector<GLchar>  buffer(infoLength + 1);
        glGetProgramInfoLog(program, infoLength, nullptr, buffer.data());

        std::string msg = util::format("Error while linking program: {}",
                buffer.data());
        throw std::runtime_error(msg);
    }
}

template <typename Iter>
GLuint create(Iter begin, Iter end) {
    GLuint program = glCreateProgram();
    for (Iter s = begin; s != end; ++ s) {
        glAttachShader(program, (*s)->getRef());
    }
    glLinkProgram(program);

    checkCreationStatus(program);
    for (Iter s = begin; s != end; ++ s) {
        glDetachShader(program, (*s)->getRef());
    }
    return program;
}


class Program: public std::enable_shared_from_this<Program> {
private:
    GLuint program_;

public:

    template <typename Container>
    Program (const Container& shaders) {
        program_ = create(begin(shaders), end(shaders));
    }

    template <typename Iter>
    Program (Iter begin, Iter end) {
        program_ = create(begin, end);
    }

    Program(std::initializer_list<ShaderPtr> shaders) {
        program_ = create(begin(shaders), end(shaders));
    }

    ~Program() {
        glDeleteProgram(program_);
    }

    GLuint getRef() const {
        return program_;
    }

    void use() const {
        glUseProgram(program_);
    }

    GLint getUniformLocation(const char* name) const {
        GLint location = glGetUniformLocation(program_, name);
        if (location != -1) {
            return location;
        } else {
            throw std::runtime_error(util::format("Cannot get location of "
                    "\"{}\" uniform variable", name));
        }
    }

    GLint getUniformLocation(const std::string& name) const {
        return getUniformLocation(name.c_str());
    }

};

typedef std::shared_ptr<Program> ProgramPtr;

ProgramPtr newProgram(std::initializer_list<ShaderPtr> shaders) {
    return std::make_shared<Program>(shaders);
}


} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_PROGRAM_HPP_ */
