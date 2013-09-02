/**
 * @file HackyRenderer.cpp
 */

#include <zephyr/gfx/HackyRenderer.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <boost/io/ios_state.hpp>

#include <fstream>

namespace zephyr {
namespace gfx {

typedef boost::io::ios_all_saver guard;


const float vertexPositions[] = {
    0.75f, 0.75f, 0.0f, 1.0f,
    0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f,
};

const char* shaderTypeToString(GLenum type) {
    switch (type) {
    case GL_VERTEX_SHADER:   return "vertex";
    case GL_FRAGMENT_SHADER: return "fragment";
    case GL_GEOMETRY_SHADER: return "geometry";
    default:
        return "(unknown)";
    }
}

GLuint createShader(GLenum type, const std::string& shaderText) {
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
        std::string msg = util::format("Error while compiling {} shader: {}",
                shaderTypeToString(type), buffer.data());
        throw std::runtime_error(msg);
    }
    return shader;
}

template <typename Container>
GLuint linkProgram(const Container& shaders) {
    GLuint program = glCreateProgram();
    for (GLuint shader : shaders) {
        glAttachShader(program, shader);
    }
    glLinkProgram(program);

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
    for (GLuint shader : shaders) {
        glDetachShader(program, shader);
    }
    return program;
}


struct Data {

    GLuint vertexBuffer;
    GLuint program;

    Data() {
        glewInit();
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        std::vector<GLuint> shaders {
            createShader(GL_VERTEX_SHADER, read_file("resources/shader.vert")),
            createShader(GL_FRAGMENT_SHADER, read_file("resources/shader.frag"))
        };

        program = linkProgram(shaders);
        for (GLuint shader : shaders) {
            glDeleteShader(shader);
        }
    }

    ~Data() {
        glDeleteBuffers(1, &vertexBuffer);
    }

    std::string read_file(const std::string& name) {
        std::ifstream in(name);
        return {
            std::istreambuf_iterator<char>(in),
            std::istreambuf_iterator<char>()
        };
    }

};


HackyRenderer::HackyRenderer(Context ctx)
: clocks(ctx.clockManager)
, data_(std::make_shared<Data>())
{
    std::cout << "[Hacky] Initializing hacky renderer" << std::endl;
    prevTime = clock.time();
    counter = 0;
    glfwSwapInterval(0);
}

void HackyRenderer::updateTime() {
    ++counter;
    double time = clock.time();
    if (time - prevTime > 1.0) {
        double fps = counter / (time - prevTime);
        {
            guard g(std::cout);
            std::cout.precision(1);
            std::cout.setf(std::ios::fixed);
            std::cout << "FPS: " << fps << std::endl;
        }
        counter = 0;
        prevTime = time;
    }
}


void HackyRenderer::update() {
    updateTime();

    int w, h;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
    glViewport(0, 0, w, h);

    glClearColor(0.4f, 0.6f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(data_->program);
    glBindBuffer(GL_ARRAY_BUFFER, data_->vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawArrays(GL_TRIANGLES, 0, 3);


//    float ratio = w / (float) h;
//    glViewport(0, 0, w, h);
//
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glRotatef(clock.time() * 100.f, 0.f, 0.f, 1.f);
//
//    glBegin(GL_TRIANGLES);
//    glColor3f(1.f, 0.f, 0.f);
//    glVertex3f(-0.6f, -0.4f, 0.f);
//    glColor3f(0.f, 1.f, 0.f);
//    glVertex3f(0.6f, -0.4f, 0.f);
//    glColor3f(0.f, 0.f, 1.f);
//    glVertex3f(0.f, 0.6f, 0.f);
//    glEnd();
}


} /* namespace gfx */
} /* namespace zephyr */

