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

#include <zephyr/effects/SimpleTerrainGenerator.hpp>

using zephyr::resources::ResourceManager;

using zephyr::effects::SimpleTerrainGenerator;


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
        glDrawElements(mode, vb->count, vb->indexType, 0);
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

namespace scene {

    struct Material {
        std::string name;
        std::string program;
    };

    struct Entity {
        std::string name;
        std::string material;
        std::string mesh;
    };

    struct Rot {
        float yaw;
        float pitch;
        float roll;
    };

    struct Pos {
        float x;
        float y;
        float z;
    };

    struct Scale {
        float sx;
        float sy;
        float sz;

        Scale(float sx = 1, float sy = 1, float sz = 1)
        : sx(sx), sy(sy), sz(sz)
        { }
    };

    struct Transform {
        Pos pos;
        Rot rot;
        Scale scale;
    };

    typedef std::unique_ptr<struct Node> NodePtr;

    struct Node {
        std::string name;
        std::string entity;
        Transform transform;
        std::vector<Node*> children;
    };

    struct SceneDescription {
        std::vector<Material> materials;
        std::vector<Entity> entities;
        std::vector<Node> nodes;
    };

}

inline constexpr float rad(float deg) {
    return deg * M_PI / 180;
}

struct SceneManager {

    ObjectPtr root;

    ShaderManager shaders;
    ProgramManager programs;
    MaterialManager materials;
    VertexArrayManager meshes;
    EntityManager entities;
    ObjectManager objects;

    Camera camera;

    GLint modelUniform;
    GLint viewUniform;
    GLint projUniform;

    static constexpr float FOV = 60.0f;
    static constexpr float zNear = 0.1f;
    static constexpr float zFar = 100.0f;

    SceneManager()
    : camera {
        Projection { FOV, 1, zNear, zFar },
        glm::vec3 { 0.0f, 0.0f, 3.0f }
    }
    , modelUniform { -1 }
    , viewUniform { -1 }
    , projUniform { -1 }
    { }

    void parseMaterial(const scene::Material& material) {
        ProgramPtr program = programs[material.program];
        materials[material.name] = newMaterial(program);
    }

    void parseEntity(const scene::Entity& entity) {
        VertexArrayPtr mesh;
        if (! entity.mesh.empty()) {
            mesh = meshes[entity.mesh];
        }
        MaterialPtr material = materials[entity.material];
        entities[entity.name] = newEntity(material, mesh);
    }

    glm::mat4 parseTransform(const scene::Transform& transform) const {
        const scene::Pos& pos = transform.pos;
        const scene::Rot& rot = transform.rot;
        const scene::Scale& scale = transform.scale;
        return glm::translate(pos.x, pos.y, pos.z) *
                glm::scale(scale.sx, scale.sy, scale.sz) *
                glm::yawPitchRoll(rot.yaw, rot.pitch, rot.roll);
    }

    void parseNode(const scene::Node& node, const std::string& parent) {
        EntityPtr entity;
        if (! node.entity.empty()) {
            entity = entities[node.entity];
        }
        glm::mat4 transform = parseTransform(node.transform);
        ObjectPtr object = newObject(entity);
        object->transform = transform;
        ObjectPtr p = objects[parent];
        p->addChild(object);
        if (! node.name.empty()) {
            objects[node.name] = object;
        }

        for (const auto& child : node.children) {
            parseNode(*child, node.name);
        }
    }

    void parseSceneDescription(const scene::SceneDescription& scene) {
        for (const auto& material : scene.materials) {
            parseMaterial(material);
        }
        for (const auto& entity : scene.entities) {
            parseEntity(entity);
        }
        for (const auto& node : scene.nodes) {
            parseNode(node, "root");
        }
    }

    ObjectPtr createScene2() {
        createProgram();
        materials["dull"] = newMaterial(programs["program"]);
        objects["root"] = newObject(nullptr);

        SimpleTerrainGenerator gen(30.0f, 320);
        meshes["quad"] = gen.create();
        meshes["star 9"] = fillVertexArray(makeStar(10, 0.3f));

        using namespace scene;
        using scene::Entity;

        Entity groundEnt { "ground", "dull", "quad" };
        Entity starEnt { "star", "dull", "star 9" };

        Node groundNode { "groundObject", "ground",
            { Pos {0, -1, 0} }
        };

        Node starNode { "starObject", "star" };
        groundNode.children.push_back(&starNode);

        Node smallStarNode { "smallStarObject", "star", {
                Pos { 0.9f, 0, 0},
                Rot { },
                Scale { 0.2f, 0.2f, 0.2f }
            }
        };
        starNode.children.push_back(&smallStarNode);

        Node leftStarNode { "leftStarObject", "star", {
                { -2.9f, 0, 0 },
                { rad(85), 0, 0 },
                { 1.2f, 1.2f, 1.2f }
            }
        };
        starNode.children.push_back(&leftStarNode);

        scene::SceneDescription scene {
            { },
            { groundEnt, starEnt },
            { groundNode }
        };

        parseSceneDescription(scene);

        return objects["root"];
    }

    ObjectPtr createScene() {
        createProgram();

        materials["dull"] = newMaterial(programs["program"]);

        ObjectPtr scene = newObject(newEntity(materials["dull"], nullptr));


        SimpleTerrainGenerator gen(30.0f, 320);
        meshes["quad"] = gen.create();

        entities["ground"] = newEntity(materials["dull"], meshes["quad"]);
        ObjectPtr ground = newObject(entities["ground"]);
        ground->transform = glm::translate<float>(0, -1, 0);
        scene->addChild(ground);


        meshes["starMesh"] = fillVertexArray(makeStar(10, 0.3f));
        entities["star"] = newEntity(materials["dull"], meshes["starMesh"]);
        ObjectPtr star = objects["root"] = newObject(entities["star"], scene);
        scene->addChild(star);

        ObjectPtr small = newObject(entities["star"], star);
        small->transform = glm::translate(0.9f, 0.0f, 0.0f) * glm::scale(0.2f, 0.2f, 0.2f);
        star->addChild(small);

        ObjectPtr left = newObject(entities["star"], scene);
        left->transform = glm::translate(-2.9f, 0.0f, 0.0f) *
                glm::scale(1.2f, 1.2f, 1.2f) *
                glm::rotate<float>(85, 0, 1, 0);
        scene->addChild(left);

        return scene;
    }

    const ObjectPtr& find(std::string path) const {
        auto pos = path.find('/');
        if (pos == std::string::npos) {
            // TODO: do sth
        } else {
            // do sth as well
        }
    }

    void update() {
        root->update();
    }

    void draw() {
        ProgramPtr program = programs["program"];
        program->use();
        setMatrix(viewUniform, camera.viewMatrix());
        setMatrix(projUniform, camera.projectionMatrix());
        drawGraph(root, modelUniform);
    }

private:

    const ObjectPtr& find(std::string& path, const ObjectPtr& root) const {
        return nullptr;
    }

    void setMatrix(GLint uniform, const glm::mat4& matrix) {
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
    }

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
, cameraController(scene->camera, clock)
, counter(0)
{
    std::cout << "[Hacky] Initializing hacky renderer" << std::endl;
    initOpenGL();
    scene->root = scene->createScene();

    core::registerHandler(ctx.dispatcher, input::msg::INPUT_SYSTEM, this,
            &HackyRenderer::inputHandler);

    core::registerHandler(ctx.dispatcher, input::msg::INPUT_SYSTEM,
            &cameraController, &CameraController::handle);

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
    cameraController.update();
    scene->update();
    scene->draw();
}



void HackyRenderer::inputHandler(const core::Message& msg) {
    using namespace zephyr::input;
    if (msg.type == msg::KEYBOARD_EVENT) {
        KeyEvent e = util::any_cast<KeyEvent>(msg.data);
        if (e.type == KeyEvent::Type::DOWN) {
            if (e.key == Key::F11) {
                glfwSwapInterval(vsync = !vsync);
            }
        }
    }
}

} /* namespace gfx */
} /* namespace zephyr */

