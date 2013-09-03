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
    0.0f, 0.5f, 0.0f, 1.0f,
    0.5f, -0.366f, 0.0f, 1.0f,
    -0.5f, -0.366f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
};

std::vector<float> makeStar(int n, float w) {
    std::vector<float> storage(2 * 4 * 3 * 4 * n);
    float* v = &storage[0];
    float* col = v +  4 * 3 * 4 *n;

    const float d = M_PI / n;

    for (int i = 0; i < 4 * 3 * n; ++i) {
        col[i * 4 + 0] = 1.0f;
        col[i * 4 + 1] = 0.0f;
        col[i * 4 + 2] = 0.0f;
        col[i * 4 + 3] = 1.0f;
    }

    for (int i = 0; i < n; ++ i) {
        float theta = 2 * i * M_PI / n;
        float x = std::sin(theta);
        float y = std::cos(theta);

        float s = 0.5f;
        float px = s * std::sin(theta - d);
        float py = s * std::cos(theta - d);

        float nx = s * std::sin(theta + d);
        float ny = s * std::cos(theta + d);

        int k = i * 4 * 3 * 4;

        v[k + 0] = 0.0f;
        v[k + 1] = 0.0f;
        v[k + 2] = w;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = px;
        v[k + 1] = py;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = x;
        v[k + 1] = y;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;


        k += 4;
        v[k + 0] = 0.0f;
        v[k + 1] = 0.0f;
        v[k + 2] = -w;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = x;
        v[k + 1] = y;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = px;
        v[k + 1] = py;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;



        k += 4;
        v[k + 0] = 0.0f;
        v[k + 1] = 0.0f;
        v[k + 2] = w;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = x;
        v[k + 1] = y;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = nx;
        v[k + 1] = ny;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;


        k += 4;
        v[k + 0] = 0.0f;
        v[k + 1] = 0.0f;
        v[k + 2] = -w;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = nx;
        v[k + 1] = ny;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;

        k += 4;
        v[k + 0] = x;
        v[k + 1] = y;
        v[k + 2] = 0.0f;
        v[k + 3] = 1.0f;

    }

    return storage;
}

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


const int N = 5;


struct Data {

    GLuint vertexBuffer;
    GLuint program;

    GLint offsetLocation;

    Data() {
        glewInit();
        initVB();
        initProgram();

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glCullFace(GL_BACK);
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

private:
    void initProgram() {
        std::vector<GLuint> shaders { createShader(GL_VERTEX_SHADER,
                read_file("resources/shader.vert")), createShader(
                GL_FRAGMENT_SHADER, read_file("resources/shader.frag")) };
        program = linkProgram(shaders);
        for (GLuint shader : shaders) {
            glDeleteShader(shader);
        }
        offsetLocation = glGetUniformLocation(program, "offset");
    }

    void initVB() {
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        std::vector<float> data = makeStar(N, 0.0f);
        for (int i = 0; i < 2 * 3 * 4 * N; ++ i) {
            std::cout << "(" << data[4 * i + 0] << ", " << data[4 * i + 1];
            std::cout << ", " << data[4 * i + 2] << ")" << std::endl;
        }
        std::cout << "size: " << data.size() << std::endl;

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0],
                GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void* offset(int n) {
    return reinterpret_cast<void*>(n);
}


void HackyRenderer::update() {
    updateTime();

    double t = clock.time();
    double dx = 0.0 * std::cos(t);
    double dy = 0.0 * std::sin(t);

    int w, h;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
    glViewport(0, 0, w, h);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(data_->program);
    glUniform2f(data_->offsetLocation, dx, dy);
    glBindBuffer(GL_ARRAY_BUFFER, data_->vertexBuffer);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, offset(3 * 4 * N * 4 * 4));

    glDrawArrays(GL_TRIANGLES, 0, 3 * 4 * N);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glUseProgram(0);


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

