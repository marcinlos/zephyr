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

#include <zephyr/core/MessageDispatcher.hpp>
#include <zephyr/input/messages.hpp>

#include <glm/glm.hpp>
#include <glm/ext.hpp>


namespace zephyr {
namespace gfx {

typedef boost::io::ios_all_saver guard;


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


struct GlobalState {
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
};

typedef std::shared_ptr<struct VertexArray> VertexArrayPtr;
typedef std::shared_ptr<struct Model> ModelPtr;
typedef std::shared_ptr<struct Object> ObjectPtr;
typedef std::weak_ptr<struct Object> WeakObjectPtr;


struct VertexArray: public std::enable_shared_from_this<VertexArray> {
    GLuint glName;
    std::size_t count;

    VertexArray(GLuint glName, std::size_t count)
    : glName(glName)
    , count(count)
    { }

    ~VertexArray() {
        glDeleteVertexArrays(1, &glName);
    }
};


template <typename... T>
VertexArrayPtr newVertexArray(T&&... args) {
    return std::make_shared<VertexArray>(std::forward<T>(args)...);
}


struct Model: public std::enable_shared_from_this<Model> {
    ProgramPtr program;
    VertexArrayPtr buffer;

    Model(ProgramPtr program, VertexArrayPtr buffer)
    : program(program)
    , buffer(buffer)
    { }

};

template <typename... T>
ModelPtr newModel(T&&... args) {
    return std::make_shared<Model>(std::forward<T>(args)...);
}


struct Object: public std::enable_shared_from_this<Object> {
    ModelPtr model;
    WeakObjectPtr parent;
    std::vector<ObjectPtr> children;

    glm::mat4 transform;
    glm::mat4 totalTransform;

    Object(ModelPtr model, WeakObjectPtr parent = WeakObjectPtr { })
    : model(model)
    , parent(parent)
    { }

    void update() {
        if (auto p = parent.lock()) {
            totalTransform = transform * p->totalTransform;
        } else {
            totalTransform = transform;
        }
        for (auto child : children) {
            child->update();
        }
    }
};

template <typename... T>
ObjectPtr newObject(T&&... args) {
    return std::make_shared<Object>(std::forward<T>(args)...);
}





VertexArrayPtr fillVertexArray(const float* data, std::size_t n) {
    GLuint vbo;
    glGenVertexArrays(1, &vbo);
    glBindVertexArray(vbo);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n, data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(2 * n));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return newVertexArray(vbo, n >> 3);
}

template <std::size_t N>
VertexArrayPtr fillVertexArray(const float (&data)[N]) {
    return fillVertexArray(data, N);
}

VertexArrayPtr fillVertexArray(const std::vector<float>& data) {
    return fillVertexArray(&data[0], data.size());
}



void drawGraph(ObjectPtr object, GLint transformLocation) {
    if (object->model) {
        const ModelPtr& model = object->model;
        const VertexArrayPtr& vb = model->buffer;

        auto data = &object->totalTransform[0][0];
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, data);
        glBindVertexArray(vb->glName);
        glDrawArrays(GL_TRIANGLES, 0, vb->count);
        glBindVertexArray(0);
    }

    for (ObjectPtr child : object->children) {
        drawGraph(child, transformLocation);
    }
}




struct Projection {
    float fov;
    float aspectRatio;
    float zNear;
    float zFar;
};

glm::mat4 projectionMatrix(const Projection& proj) {
    return glm::perspective(proj.fov, proj.aspectRatio, proj.zNear, proj.zFar);
}



struct Data {

    ObjectPtr root;

    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;

    GLint modelUniform;
    GLint viewUniform;
    GLint projUniform;

    Data() {
        initOpenGL();
        root = createRoot();
    }

    ObjectPtr createRoot() {
        return newObject(
            newModel(
                createProgram(),
                fillVertexArray(makeStar(9, 0.3f))
            )
        );
    }

    void initOpenGL() {
        glewInit();

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glCullFace(GL_BACK);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    void update(double t) {
        const double r = 0.6;
        float dx = r * std::cos(t);
        float dy = r * std::sin(t);

        root->transform = glm::rotate<float>(t * 20, 0, 1, 0);// * glm::rotate<float>(t * 50, 0, 0, 1);
        root->update();
    }

    void draw() {

        viewMatrix = glm::translate(0.0f, 0.0f, -2.0f);

        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projUniform, 1, GL_FALSE, &projMatrix[0][0]);
        drawGraph(root, modelUniform);
    }

private:
    ProgramPtr createProgram() {
        ProgramPtr program(new Program {
            newVertexShader("resources/shader.vert"),
            newFragmentShader("resources/shader.frag")
        });
        viewUniform = program->getUniformLocation("viewMatrix");
        modelUniform = program->getUniformLocation("modelMatrix");
        projUniform = program->getUniformLocation("projectionMatrix");
        return program;
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

    core::registerHandler(ctx.dispatcher, input::msg::INPUT_SYSTEM, this,
            &HackyRenderer::inputHandler);
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
    double t = 2 * clock.time();

    int w, h;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
    glViewport(0, 0, w, h);

    float ratio = w / (float) h;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT);

    glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    data_->root->model->program->use();

    data_->projMatrix = projectionMatrix({
        60.0f, ratio, 1.0f, 10.0f
    });

    data_->update(t);
    data_->draw();

}

void HackyRenderer::inputHandler(const core::Message& msg) {
//    std::cout << "[HackyRenderer] " << msg << std::endl;
}

} /* namespace gfx */
} /* namespace zephyr */

