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


#include <zephyr/input/KeyEvent.hpp>
#include <zephyr/input/ButtonEvent.hpp>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

#include <list>


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


typedef std::shared_ptr<struct VertexArray> VertexArrayPtr;
typedef std::shared_ptr<struct Entity> EntityPtr;
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


struct Entity: public std::enable_shared_from_this<Entity> {
    ProgramPtr program;
    VertexArrayPtr buffer;

    Entity(ProgramPtr program, VertexArrayPtr buffer)
    : program(program)
    , buffer(buffer)
    { }

};

template <typename... T>
EntityPtr newEntity(T&&... args) {
    return std::make_shared<Entity>(std::forward<T>(args)...);
}


struct Object: public std::enable_shared_from_this<Object> {
    EntityPtr entity;
    WeakObjectPtr parent;
    std::vector<ObjectPtr> children;

    glm::mat4 transform;
    glm::mat4 totalTransform;


    Object(EntityPtr entity, WeakObjectPtr parent = WeakObjectPtr { })
    : entity(entity)
    , parent(parent)
    { }

    void update() {
        if (auto p = parent.lock()) {
            totalTransform = p->totalTransform * transform;
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

template <typename T>
struct no_loader {

    T load(const std::string& name) {
        throw std::runtime_error("No such resource");
    }

};

template <typename T, typename Loader = no_loader<T>>
class ResourceManager {
private:
    std::unordered_map<std::string, T> resources;

    Loader loader;

    struct Proxy {
        ResourceManager& manager;
        std::string name;

        Proxy(ResourceManager& manager, std::string name)
        : manager(manager)
        , name(std::move(name))
        { }

        operator T& () {
            return manager.doGet(name);
        }

        Proxy& operator = (const T& value) {
            manager.put(name, value);
            return *this;
        }
    };

    T& doGet(const std::string& name) {
        auto result = resources.find(name);
        if (result != end(resources)) {
            return result->second;
        } else {
            T value = loader.load(name);
            return put(name, value);
        }
    }

public:
    T& put(const std::string& name, const T& value) {
        return resources[name] = value;
    }

    const T& get(const std::string& name) const {
        return const_cast<ResourceManager&>(*this).doGet(name);
    }

    const T& operator [](const std::string& name) const {
        return get(name);
    }

    Proxy operator [](std::string name) {
        return Proxy(*this, std::move(name));
    }
};




class ShaderManager: public ResourceManager<ShaderPtr> {

};

class ProgramManager: public ResourceManager<ProgramPtr> {

};

class VertexArrayManager: public ResourceManager<VertexArrayPtr> {

};

class EntityManager: public ResourceManager<EntityPtr> {

};

class ObjectManager: public ResourceManager<ObjectPtr> {

};


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
    if (object->entity) {
        const EntityPtr& model = object->entity;
        const VertexArrayPtr& vb = model->buffer;

        auto data = glm::value_ptr(object->totalTransform);
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

    glm::mat4 matrix() const {
        return glm::perspective(fov, aspectRatio, zNear, zFar);
    }
};

const glm::vec3 FWD   {  0,  0, -1 };
const glm::vec3 BACK  {  0,  0,  1 };
const glm::vec3 UP    {  0,  1,  0 };
const glm::vec3 DOWN  {  0, -1,  0 };
const glm::vec3 LEFT  { -1,  0,  0 };
const glm::vec3 RIGHT {  1,  0,  0 };

struct Camera {
    Projection proj;
    glm::vec3 pos;
//    glm::quat dir;
    glm::mat4 rot;

    Camera()
//    : dir(0, 0, 0, -1)
    { }

    glm::vec3 dir() const {
        return dirFromView(FWD);
    }

    glm::vec3 dirToView(const glm::vec3& v) const {
        return glm::vec3 { rot * glm::vec4(v, 0) };
    }

    glm::vec3 dirFromView(const glm::vec3& v) const {
        return glm::vec3 { glm::inverse(rot) * glm::vec4(v, 0) };
    }
};



struct SceneManager {

    ObjectPtr root;

    ShaderManager shaders;
    ProgramManager programs;
    VertexArrayManager vaos;
    EntityManager entities;
    ObjectManager objects;

    Camera camera;
    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;

    GLint modelUniform;
    GLint viewUniform;
    GLint projUniform;

    SceneManager() {
        initOpenGL();
        root = createScene();
        setupCamera();
    }

    ObjectPtr createScene() {
        createProgram();

        vaos["star 9"] = fillVertexArray(makeStar(10, 0.3f));
        entities["star"] = newEntity(programs["program"], vaos["star 9"]);
        ObjectPtr root = objects["root"] = newObject(entities["star"]);

        ObjectPtr small = newObject(entities["star"], root);
        small->transform = glm::translate(0.9f, 0.0f, 0.0f) * glm::scale(0.2f, 0.2f, 0.2f);
        root->children.push_back(small);

        ObjectPtr left = newObject(entities["star"], root);
        small->transform = glm::translate(0.9f, 0.0f, 0.0f) * glm::scale(0.2f, 0.2f, 0.2f);
        root->children.push_back(small);

        return root;
    }

    void setupCamera() {
        camera.pos = glm::vec3 { 0.0f, 0.0f, -3.0f };
    }

    void initOpenGL() {
        glewInit();

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glCullFace(GL_BACK);

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glDepthRange(0.0f, 1.0f);

    }

    void update() {
        viewMatrix = camera.rot * glm::translate(camera.pos);/* glm::mat4_cast(camera.dir); */
        root->update();
    }

    void draw() {
        root->entity->program->use();
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projUniform, 1, GL_FALSE, glm::value_ptr(projMatrix));
        drawGraph(root, modelUniform);
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
{
    std::cout << "[Hacky] Initializing hacky renderer" << std::endl;
    prevTime = clock.time();
    counter = 0;
    glfwSwapInterval(vsync);

    core::registerHandler(ctx.dispatcher, input::msg::INPUT_SYSTEM, this,
            &HackyRenderer::inputHandler);

    MatrixRotator rotator { 30, glm::vec3 { 0, 1, 0 } };
//    taskletScheduler.add(changer(scene->root->transform, rotator));

    Action action = repeatedly(actionScheduler, [this](){
        double time = clock.time();
        double fps = counter / (time - prevTime);
        {
            guard g(std::cout);
            std::cout.precision(1);
            std::cout.setf(std::ios::fixed);
            std::cout << "FPS: " << fps << std::endl;
        }
        counter = 0;
        prevTime = time;
    }, 1.0);

    actionScheduler.scheduleIn(action, 1.0);
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
    glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
    glViewport(0, 0, w, h);

    float ratio = w / (float) h;

    glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->projMatrix = Projection{
        60.0f, ratio, 1.0f, 100.0f
    }.matrix();

    using input::Key;

    const float v = 4.0f;
    const float ds = v * clock.dt();

    const float hRotV = 60;
    const float hRotH = 60;

    if (pressed(Key::W)) {
        scene->camera.pos -= ds * scene->camera.dirFromView(FWD);
    }
    if (pressed(Key::S)) {
        scene->camera.pos -= ds * scene->camera.dirFromView(BACK);
    }
    if (pressed(Key::A)) {
        scene->camera.pos -= ds * scene->camera.dirFromView(LEFT);
    }
    if (pressed(Key::D)) {
        scene->camera.pos -= ds * scene->camera.dirFromView(RIGHT);
    }
    if (pressed(Key::LEFT)) {
        scene->camera.rot = glm::rotate<float>(clock.dt() * -hRotH, 0, 1, 0) * scene->camera.rot;
        std::cout << scene->camera.dirFromView(FWD) << std::endl;
        std::cout << scene->camera.pos << std::endl;
    }
    if (pressed(Key::RIGHT)) {
        scene->camera.rot = glm::rotate<float>(clock.dt() * hRotH, 0, 1, 0) * scene->camera.rot;
        std::cout << scene->camera.dirFromView(FWD) << std::endl;
        std::cout << scene->camera.pos << std::endl;
    }
    if (pressed(Key::UP)) {
        scene->camera.rot = glm::rotate<float>(clock.dt() * -hRotV, 1, 0, 0) * scene->camera.rot;
        std::cout << scene->camera.pos << std::endl;
    }
    if (pressed(Key::DOWN)) {
        scene->camera.rot = glm::rotate<float>(clock.dt() * hRotV, 1, 0, 0) * scene->camera.rot;
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
                std::cout << "dir: " << scene->camera.dirToView(FWD) << std::endl;
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
//        std::cout << "old: " << cursor.x
//        std::cout << "YAAY! " << dx * dx + dy * dy << std::endl;
//        if (dx * dx + dy * dy < 100) {
//            scene->camera.rot = glm::lookAt(
//                    glm::vec3{0, 0, 0},
//                    glm::vec3{dx, dy, z},
//                    glm::vec3{0, 1, 0});
//        }
        cursor = pos;
    }

}

} /* namespace gfx */
} /* namespace zephyr */

