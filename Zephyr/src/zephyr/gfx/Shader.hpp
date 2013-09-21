/**
 * @file Shader.hpp
 */

#ifndef ZEPHYR_GFX_SHADER_HPP_
#define ZEPHYR_GFX_SHADER_HPP_

#include <zephyr/util/format.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>


namespace zephyr {
namespace gfx {

typedef std::shared_ptr<class Shader> ShaderPtr;

inline const char* shaderTypeToString(GLenum type) {
    switch (type) {
    case GL_VERTEX_SHADER:   return "vertex";
    case GL_FRAGMENT_SHADER: return "fragment";
    case GL_GEOMETRY_SHADER: return "geometry";
    default:
        return "(unknown)";
    }
}


inline std::string readFile(const std::string& name) {
    std::ifstream in(name);
    return {
        std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>()
    };
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


class ShaderBuilder {
public:

    ShaderBuilder(GLenum type)
    : type { type }
    { }

    ShaderBuilder& version(int version = 330) {
        ss << "#version " << 330 << std::endl;
        return *this;
    }

    ShaderBuilder& append(const std::string& text) {
        return append(text.c_str());
    }

    ShaderBuilder& append(const char* text) {
        ss << text << std::endl;
        return *this;
    }

    ShaderBuilder& file(const char* path) {
        std::ifstream in(path);
        if (!in) {
            throw std::runtime_error(util::format("Cannot open file {}", path));
        }
        std::copy(std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>(),
                std::ostreambuf_iterator<char>(ss));
        return *this;
    }

    ShaderBuilder& file(const std::string& path) {
        return file(path.c_str());
    }

    ShaderBuilder& define(const std::string& text) {
        return define(text.c_str());
    }

    ShaderBuilder& define(const char* text) {
        ss << "#define " << text << std::endl;
        return *this;
    }

    ShaderPtr create() {
        std::clog << "********************\n" << ss.str() << std::endl;
        GLuint id = createShader(type, ss.str());
        return std::make_shared<Shader>(id);
    }


private:
    std::stringstream ss;
    GLenum type;
};


class Shader: public std::enable_shared_from_this<Shader> {
public:
    Shader(GLenum type, const std::string& path)
    : Shader { createShader(type, readFile(path)) }
    { }

    Shader(GLuint shader)
    : shader_ { shader }
    { }

    ~Shader() {
        glDeleteShader(shader_);
    }

    GLuint ref() const {
        return shader_;
    }

private:
    GLuint shader_;
};


inline ShaderPtr newVertexShader(const std::string& path) {
    return std::make_shared<Shader>(GL_VERTEX_SHADER, path);
}

inline ShaderPtr newFragmentShader(const std::string& path) {
    return std::make_shared<Shader>(GL_FRAGMENT_SHADER, path);
}


} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_SHADER_HPP_ */
