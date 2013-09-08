/**
 * @file HackyRenderer.cpp
 */

#include <zephyr/gfx/HackyRenderer.hpp>
#include <zephyr/resources/ResourceManager.hpp>

#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <boost/io/ios_state.hpp>

#include <zephyr/gfx/Camera.hpp>

#include <zephyr/gfx/star.hpp>

#include <zephyr/gfx/Shader.hpp>
#include <zephyr/gfx/Program.hpp>

#include <zephyr/core/MessageDispatcher.hpp>
#include <zephyr/input/messages.hpp>


#include <zephyr/input/KeyEvent.hpp>
#include <zephyr/input/ButtonEvent.hpp>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

#include <zephyr/gfx/objects.h>
#include <list>

using zephyr::resources::ResourceManager;


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


VertexArrayPtr generateTerrain() {

    const float vertices[] = {
        -1, 0,  1, 1,
        -1, 0, -1, 1,
         1, 0, -1, 1,
         1, 0,  1, 1,

         1, 0, 0, 1,
         0, 1, 0, 1,
         0, 0, 1, 1,
         1, 1, 1, 1
    };

    const std::uint16_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    const std::size_t vertexSize = 4 * sizeof(float);
    const std::size_t colorSize = 4 * sizeof(float);
    const std::size_t indexSize = sizeof(std::uint16_t);

    std::size_t vertexCount = sizeof(vertices) / (vertexSize + colorSize);
    std::size_t indexCount = sizeof(indices) / indexSize;

    void* colorOffset = reinterpret_cast<void*>(vertexCount * vertexSize);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertex vertexBuffer
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, colorOffset);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Index buffer
    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindVertexArray(0);

    glDeleteBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return newVertexArray(vao, indexCount, true);
}

void drawGraph(ObjectPtr object, GLint transformLocation) {
    if (object->entity) {
        const EntityPtr& model = object->entity;
        if (const VertexArrayPtr& vb = model->buffer) {

            auto data = glm::value_ptr(object->totalTransform);
            glUniformMatrix4fv(transformLocation, 1, GL_FALSE, data);
            glBindVertexArray(vb->glName);
            if (vb->indexed) {
                glDrawElements(GL_TRIANGLES, vb->count, GL_UNSIGNED_SHORT, 0);
            } else {
                glDrawArrays(GL_TRIANGLES, 0, vb->count);
            }
            glBindVertexArray(0);
        }
    }

    for (ObjectPtr child : object->children) {
        drawGraph(child, transformLocation);
    }
}



struct SceneManager {

    ObjectPtr root;

    ShaderManager shaders;
    ProgramManager programs;
    VertexArrayManager meshes;
    EntityManager entities;
    ObjectManager objects;

    Camera camera;

    GLint modelUniform;
    GLint viewUniform;
    GLint projUniform;

    const float FOV = 60.0f;
    const float zNear = 0.1f;
    const float zFar = 100.0f;

    SceneManager() {
//        root = createScene();
        setupCamera();
    }

    ObjectPtr createScene() {
        createProgram();

        ObjectPtr scene = newObject(newEntity(programs["program"], nullptr));

        meshes["quad"] = generateTerrain();/*fillVertexArray({
           -1, -1, 0, 1,
           -1,  1, 0, 1,
            1, -1, 0, 1,

            1, -1, 0, 1,
           -1,  1, 0, 1,
            1,  1, 0, 1,

           0.2f, 0, 0, 1,
           0.2f, 0, 0, 1,
           0.2f, 0, 0, 1,
           0.2f, 0, 0, 1,
           0.2f, 0, 0, 1,
           0.2f, 0, 0, 1,
        });*/

        entities["ground"] = newEntity(programs["program"], meshes["quad"]);
        ObjectPtr ground = newObject(entities["ground"]);
        const float size = 100.0f;
        ground->transform =
                glm::translate<float>(0, -1, 0);// *
//                glm::rotate<float>(-90, 1, 0, 0) *
//                glm::scale(size, size, size);
        scene->addChild(ground);


        meshes["star 9"] = fillVertexArray(makeStar(10, 0.3f));
        entities["star"] = newEntity(programs["program"], meshes["star 9"]);
        ObjectPtr root = objects["root"] = newObject(entities["star"], scene);
        scene->addChild(root);

        ObjectPtr small = newObject(entities["star"], root);
        small->transform = glm::translate(0.9f, 0.0f, 0.0f) * glm::scale(0.2f, 0.2f, 0.2f);
        root->addChild(small);

        ObjectPtr left = newObject(entities["star"], root);
        small->transform = glm::translate(0.9f, 0.0f, 0.0f) * glm::scale(0.2f, 0.2f, 0.2f);
        root->addChild(small);


        return scene;
    }

    void setupCamera() {
        camera.projection({ FOV, 1, zNear, zFar });
        camera.pos = glm::vec3 { 0.0f, 0.0f, 3.0f };
    }

    void update() {
        root->update();
    }

    void draw() {
        root->entity->program->use();
        setMatrix(viewUniform, camera.viewMatrix());
        setMatrix(projUniform, camera.projectionMatrix());
        drawGraph(root, modelUniform);
    }

    void setMatrix(GLint uniform, const glm::mat4& matrix) {
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
    }

private:
    void createProgram() {

        shaders["vertex"] = newVertexShader("resources/shader.vert");
        shaders["fragment"] = newFragmentShader("resources/shader.frag");

        programs["program"] = newProgram({
            shaders["vertex"],
            shaders["fragment"]
        });
        ProgramPtr program = programs["program"];
        viewUniform = program->getUniformLocation("viewMatrix");
        modelUniform = program->getUniformLocation("modelMatrix");
        projUniform = program->getUniformLocation("projectionMatrix");
    }
};

typedef std::shared_ptr<SceneManager> SceneManagerPtr;


class Client  {
public:

    Client(SceneManagerPtr scene)
    : scene(std::move(scene))
    { }

private:
    SceneManagerPtr scene;

    ShaderManager shaders;
    ProgramManager programs;
    VertexArrayManager meshes;
    EntityManager entities;
    ObjectManager objects;

};


template <typename T, typename Fun>
struct ValueChanger {

    T& value;
    Fun fun;

    ValueChanger(T& value, Fun fun = { })
    : value(value)
    , fun(fun)
    { }

    bool operator () (double t, double dt) {
        return fun(value, t, dt);
    }

};

template <typename T, typename Fun>
ValueChanger<T, Fun> changer(T& value, Fun fun = { }) {
    return ValueChanger<T, Fun>(value, fun);
}

glm::mat4 rotate(float angle, const glm::vec3& axis) {
    return glm::rotate(angle, axis);
}

struct MatrixRotator {
    float radiansPerSecond;
    glm::vec3 axis;

    bool operator () (glm::mat4& value, double, double dt) {
        value *= rotate(dt * radiansPerSecond, axis);
        return true;
    }
};

struct MatrixTranslator {
    glm::vec3 displacement;

    bool operator () (glm::mat4& value, double, float dt) {
        value *= glm::translate(dt * displacement);
        return true;
    }
};


HackyRenderer::HackyRenderer(Context ctx)
: clocks(ctx.clockManager)
, clock(clocks.getMainClock())
, scene(std::make_shared<SceneManager>())
, counter(0)
{
    std::cout << "[Hacky] Initializing hacky renderer" << std::endl;
    initOpenGL();
    scene->root = scene->createScene();

    core::registerHandler(ctx.dispatcher, input::msg::INPUT_SYSTEM, this,
            &HackyRenderer::inputHandler);

    MatrixRotator rotator { 30, glm::vec3 { 0, 1, 0 } };
//    taskletScheduler.add(changer(scene->root->transform, rotator));

    time::Action action = repeatedly(actionScheduler, [this](){
        double time = clock.time();
        double fps = counter;
        {
            guard g(std::cout);
            std::cout.precision(1);
            std::cout.setf(std::ios::fixed);
            std::cout << "FPS: " << fps << std::endl;
        }
        counter = 0;
    }, 1.0);

    actionScheduler.scheduleIn(action, 1.0);
}

void HackyRenderer::initOpenGL() {
    glewInit();
    glfwSwapInterval(vsync);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glDepthRange(0.0f, 1.0f);

}

void HackyRenderer::updateTime() {
    ++counter;
    double time = clock.time();
    taskletScheduler.update(time, clock.dt());
    actionScheduler.update(time);
}


void HackyRenderer::update() {
    updateTime();

    int w, h;
    GLFWwindow* window = glfwGetCurrentContext();
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    float ratio = w / (float) h;

    glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->camera.adjustRatio(ratio);


    using input::Key;

    const float v = 4.0f;
    const float ds = v * clock.dt();

    const float hRotV = 60;
    const float hRotH = 60;

    if (pressed(Key::W)) {
        scene->camera.pos += ds * scene->camera.dirFromView(FWD);
    }
    if (pressed(Key::S)) {
        scene->camera.pos += ds * scene->camera.dirFromView(BACK);
    }
    if (pressed(Key::A)) {
        scene->camera.pos += ds * scene->camera.dirFromView(LEFT);
    }
    if (pressed(Key::D)) {
        scene->camera.pos += ds * scene->camera.dirFromView(RIGHT);
    }
    if (pressed(Key::E)) {
        scene->camera.pos += ds * scene->camera.dirFromView(UP);
    }
    if (pressed(Key::Q)) {
        scene->camera.pos += ds * scene->camera.dirFromView(DOWN);
    }

    if (pressed(Key::LEFT)) {
        scene->camera.rotate(clock.dt() * -hRotH, 0, UP);
        std::cout << scene->camera.dirFromView(FWD) << std::endl;
        std::cout << scene->camera.pos << std::endl;
    }
    if (pressed(Key::RIGHT)) {
        scene->camera.rotate(clock.dt() * hRotH, 0, UP);
        std::cout << scene->camera.dirFromView(FWD) << std::endl;
        std::cout << scene->camera.pos << std::endl;
    }
    if (pressed(Key::UP)) {
        scene->camera.rotate(0, clock.dt() * hRotV, UP);
        std::cout << scene->camera.pos << std::endl;
    }
    if (pressed(Key::DOWN)) {
        scene->camera.rotate(0, clock.dt() * -hRotV, UP);
        std::cout << scene->camera.pos << std::endl;
    }
    scene->update();
    scene->draw();

}


void HackyRenderer::inputHandler(const core::Message& msg) {
//    std::cout << "[HackyRenderer] " << msg << std::endl;
    using namespace zephyr::input;

    if (msg.type == msg::KEYBOARD_EVENT) {
        KeyEvent e = util::any_cast<KeyEvent>(msg.data);
        if (e.type == KeyEvent::Type::DOWN) {
            isPressed[static_cast<int>(e.key)] = true;

            if (e.key == Key::SPACE) {
                std::cout << "dir: " << scene->camera.dirFromView(FWD) << std::endl;
                std::cout << "pos: " << scene->camera.pos << std::endl;
            }
            else if (e.key == Key::F11) {
                glfwSwapInterval(vsync = !vsync);
            }

        } else if (e.type == KeyEvent::Type::UP) {
            isPressed[static_cast<int>(e.key)] = false;
        }
    } else if (msg.type == msg::CURSOR_EVENT) {
        Position pos = util::any_cast<Position>(msg.data);
        float dx = pos.x - cursor.x;
        float dy = - (pos.y - cursor.y);
        float z = -100.0f;

        if (dx * dx + dy * dy < 500) {
            float s = 0.5f;
            scene->camera.rotate(s * dx, s * dy, UP);
        }
        cursor = pos;
    }

}

} /* namespace gfx */
} /* namespace zephyr */

