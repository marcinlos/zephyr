/**
 * @file HackyRenderer.cpp
 */

#include <zephyr/gfx/HackyRenderer.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <boost/io/ios_state.hpp>


#include <zephyr/gfx/star.hpp>

#include <zephyr/gfx/Shader.hpp>
#include <zephyr/gfx/Program.hpp>


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

struct Uniform {
    virtual void set(GLint location) = 0;
    virtual ~Uniform() = default;
};

struct Uniform1f: Uniform {
    GLfloat v0;

    void set(GLint location) override {
        glUniform1f(location, v0);
    }
};

struct Uniform2f: Uniform {
    GLfloat v0;
    GLfloat v1;

    void set(GLint location) override {
        glUniform2f(location, v0, v1);
    }
};



const int N = 5;


void* offset(int vertices) {
    std::size_t bytes = 3 * 4 * sizeof(float) * vertices;
    return reinterpret_cast<void*>(bytes);
}

struct Element {
    ProgramPtr program;
    GLuint buffer;
    GLsizei count;



};

void draw(const Element& e) {
//    glUseProgram(e.program);
    glBindBuffer(GL_ARRAY_BUFFER, e.buffer);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, offset(e.count));
    
    glDrawArrays(GL_TRIANGLES, 0, 3 * e.count);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glUseProgram(0);
}


struct Data {

    Element element;

    GLint offsetLocation;
    GLint zNearLocation;
    GLint zFarLocation;
    GLint frustumScaleLocation;

    Data() {
        glewInit();
        element = {initProgram(), initVB(), 4 * N};

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glCullFace(GL_BACK);
    }

    ~Data() {
        glDeleteBuffers(1, &element.buffer);
    }

private:
    ProgramPtr initProgram() {
        std::vector<ShaderPtr> shaders {
            std::make_shared<Shader>(GL_VERTEX_SHADER, "resources/shader.vert"),
            std::make_shared<Shader>(GL_FRAGMENT_SHADER, "resources/shader.frag")
        };
        ProgramPtr program = std::make_shared<Program>(shaders);
        offsetLocation = program->getUniformLocation("offset");
        zNearLocation = program->getUniformLocation("zNear");
        zFarLocation = program->getUniformLocation("zFar");
        frustumScaleLocation = program->getUniformLocation("frustumScale");
        return program;
    }

    GLuint initVB() {
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        std::vector<float> data = makeStar(N, 0.0f);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0],
                GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return buffer;
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

    double t = clock.time();
    double dx = 0.3 * std::cos(t);
    double dy = 0.3 * std::sin(t);

    int w, h;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
    glViewport(0, 0, w, h);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    data_->element.program->use();

    Uniform2f offset;
    offset.v0 = dx;
    offset.v1 = dy;

    offset.set(data_->offsetLocation);

//    glUniform2f(data_->offsetLocation, dx, dy);
    glUniform1f(data_->frustumScaleLocation, 1.0f);
    glUniform1f(data_->zNearLocation, 1.0f);
    glUniform1f(data_->zFarLocation, 10.0f);

    draw(data_->element);


//    float ratio = w / (float) h;
//    glViewport(0, 0, w, h);

}


} /* namespace gfx */
} /* namespace zephyr */

