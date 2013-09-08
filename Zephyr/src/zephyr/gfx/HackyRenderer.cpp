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

#include <zephyr/gfx/BufferGenerator.hpp>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

#include <zephyr/gfx/objects.h>
#include <list>

#include <zephyr/effects/TerrainGenerator.hpp>

using zephyr::resources::ResourceManager;

using zephyr::effects::TerrainGenerator;


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


void drawBuffer(const VertexArrayPtr& vb) {
    glBindVertexArray(vb->glName);
    GLenum mode = primitiveToGL(vb->mode);
    if (vb->indexed) {
        glDrawElements(mode, vb->count, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(mode, 0, vb->count);
    }
    glBindVertexArray(0);
}

void drawGraph(ObjectPtr object, GLint transformLocation) {
    if (object->entity) {
        const EntityPtr& model = object->entity;
        if (const VertexArrayPtr& vb = model->buffer) {
            auto data = glm::value_ptr(object->totalTransform);
            glUniformMatrix4fv(transformLocation, 1, GL_FALSE, data);
            drawBuffer(vb);
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

        TerrainGenerator gen(30.0f, 320);
        meshes["quad"] = gen.create();

        entities["ground"] = newEntity(programs["program"], meshes["quad"]);
        ObjectPtr ground = newObject(entities["ground"]);
        ground->transform = glm::translate<float>(0, -1, 0);
        scene->addChild(ground);


        meshes["star 9"] = fillVertexArray(makeStar(10, 0.3f));
        entities["star"] = newEntity(programs["program"], meshes["star 9"]);
        ObjectPtr root = objects["root"] = newObject(entities["star"], scene);
        scene->addChild(root);

        ObjectPtr small = newObject(entities["star"], root);
        small->transform = glm::translate(0.9f, 0.0f, 0.0f) * glm::scale(0.2f, 0.2f, 0.2f);
        root->addChild(small);

        ObjectPtr left = newObject(entities["star"], root);
        small->transform = glm::translate(-2.9f, 0.0f, 0.0f) *
                glm::scale(1.2f, 1.2f, 1.2f) *
                glm::rotate<float>(85, 0, 1, 0);
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

    bool operator () (glm::mat4& value, double, double dt) const {
        value *= rotate(dt * radiansPerSecond, axis);
        return true;
    }
};

struct MatrixTranslator {
    glm::vec3 displacement;

    bool operator () (glm::mat4& value, double, float dt) const {
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

    if (input[Key::W]) {
        scene->camera.pos += ds * scene->camera.dirFromView(FWD);
    }
    if (input[Key::S]) {
        scene->camera.pos += ds * scene->camera.dirFromView(BACK);
    }
    if (input[Key::A]) {
        scene->camera.pos += ds * scene->camera.dirFromView(LEFT);
    }
    if (input[Key::D]) {
        scene->camera.pos += ds * scene->camera.dirFromView(RIGHT);
    }
    if (input[Key::E]) {
        scene->camera.pos += ds * scene->camera.dirFromView(UP);
    }
    if (input[Key::Q]) {
        scene->camera.pos += ds * scene->camera.dirFromView(DOWN);
    }

    if (input[Key::LEFT]) {
        scene->camera.rotate(clock.dt() * -hRotH, 0, UP);
        std::cout << scene->camera.dirFromView(FWD) << std::endl;
        std::cout << scene->camera.pos << std::endl;
    }
    if (input[Key::RIGHT]) {
        scene->camera.rotate(clock.dt() * hRotH, 0, UP);
        std::cout << scene->camera.dirFromView(FWD) << std::endl;
        std::cout << scene->camera.pos << std::endl;
    }
    if (input[Key::UP]) {
        scene->camera.rotate(0, clock.dt() * hRotV, UP);
        std::cout << scene->camera.pos << std::endl;
    }
    if (input[Key::DOWN]) {
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
            input[e.key] = true;

            if (e.key == Key::SPACE) {
                std::cout << "dir: " << scene->camera.dirFromView(FWD) << std::endl;
                std::cout << "pos: " << scene->camera.pos << std::endl;
            }
            else if (e.key == Key::F11) {
                glfwSwapInterval(vsync = !vsync);
            }

        } else if (e.type == KeyEvent::Type::UP) {
            input[e.key] = false;
        }
    } else if (msg.type == msg::BUTTON_EVENT) {
        ButtonEvent e = util::any_cast<ButtonEvent>(msg.data);
        input[e.button] = (e.type == ButtonEvent::Type::DOWN);
    } else if (msg.type == msg::CURSOR_EVENT) {
        const float sensitivity = 0.5f;
        const float moveScale = 0.3f;
        const float rotScale = 1.0f;

        Position pos = util::any_cast<Position>(msg.data);
        float dx =   sensitivity * (pos.x - input.mouse().x);
        float dy = - sensitivity * (pos.y - input.mouse().y);
        if (input[Button::RIGHT]) {
            auto viewLeft = scene->camera.dirFromView(RIGHT);
            auto viewUp = scene->camera.dirFromView(UP);
            scene->camera.pos += moveScale * (dx * viewLeft + dy * viewUp);
        } else {
            float z = -100.0f;
            dx *= rotScale;
            dy *= rotScale;
            if (dx * dx + dy * dy < 500) {
                scene->camera.rotate(dx, dy, UP);
            }
        }
        input.mouse(pos);
    } else if (msg.type == msg::SCROLL_EVENT) {
        float scroll = util::any_cast<double>(msg.data);
        scene->camera.pos += scroll * scene->camera.forward();
    }

}

} /* namespace gfx */
} /* namespace zephyr */

