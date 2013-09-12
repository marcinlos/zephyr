/**
 * @file Shader.hpp
 */

#ifndef ZEPHYR_GFX_SHADER_HPP_
#define ZEPHYR_GFX_SHADER_HPP_

#include <GL/glew.h>
#include <GL/gl.h>
#include <fstream>
#include <memory>


namespace zephyr {
namespace gfx {


inline const char* shaderTypeToString(GLenum type) {
    switch (type) {
    case GL_VERTEX_SHADER:   return "vertex";
    case GL_FRAGMENT_SHADER: return "fragment";
    case GL_GEOMETRY_SHADER: return "geometry";
    default:
        return "(unknown)";
    }
}

inline GLuint createShader(GLenum type, const std::string& shaderText) {
    GLuint shader = glCreateShader(type);
    const char* text = shaderText.c_str();
    glShaderSource(shader, 1, &text, nullptr);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        GLint infoLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
        std::vector<GLchar> buffer(infoLength + 1);
        glGetShaderInfoLog(shader, infoLength, nullptr, buffer.data());

        throw std::runtime_error(util::format(
                "Error while compiling {} shader: {}", shaderTypeToString(type),
                buffer.data()));
    }
    return shader;
}

std::string readFile(const std::string& name) {
    std::ifstream in(name);
    return {
        std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>()
    };
}

class Shader: public std::enable_shared_from_this<Shader> {
private:
    GLuint shader_;

public:
    Shader(GLenum type, const std::string& path)
    : shader_(createShader(type, readFile(path)))
    { }

    ~Shader() {
        glDeleteShader(shader_);
    }

    GLuint ref() const {
        return shader_;
    }

};

typedef std::shared_ptr<Shader> ShaderPtr;

inline ShaderPtr newVertexShader(const std::string& path) {
    return std::make_shared<Shader>(GL_VERTEX_SHADER, path);
}

inline ShaderPtr newFragmentShader(const std::string& path) {
    return std::make_shared<Shader>(GL_FRAGMENT_SHADER, path);
}


} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_SHADER_HPP_ */
