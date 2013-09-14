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

#include <zephyr/effects/SimpleTerrainGenerator.hpp>
#include <zephyr/gfx/Mesh.hpp>

#include <zephyr/gfx/uniforms.hpp>

using zephyr::resources::ResourceManager;

using zephyr::effects::SimpleTerrainGenerator;


namespace zephyr {
namespace gfx {

typedef boost::io::ios_all_saver guard;


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

struct SceneManager_ {

    ObjectPtr root;

    ShaderManager shaders;
    ProgramManager programs;
    MaterialManager materials;
    VertexArrayManager meshes;
    EntityManager entities;
    ObjectManager objects;

    Camera camera;

    glm::vec3 sunDirection;
    float sunIntensity;
    float ambient;

    static constexpr float FOV = 60.0f;
    static constexpr float zNear = 0.1f;
    static constexpr float zFar = 100.0f;

    SceneManager_()
    : camera {
        Projection { FOV, 1, zNear, zFar },
        glm::vec3 { 0.0f, 0.0f, 3.0f }
    }

    , sunIntensity { 1.5f }
    , ambient { 0.2f }
    { }

    void parseMaterial(const scene::Material& material) {
        ProgramPtr program = programs[material.program];
        materials[material.name] = newMaterial(program);
    }

    void parseEntity(const scene::Entity& entity) {
        MeshPtr mesh;
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

        SimpleTerrainGenerator gen(100.0f, 8, 45.0f);
        meshes["quad"] = gen.create();
        meshes["star 9"] = makeStar(10, 0.3f);

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


        SimpleTerrainGenerator gen(100.0f, 8, 25.0f);
        meshes["quad"] = gen.create();

        entities["ground"] = newEntity(materials["dull"], meshes["quad"]);
        ObjectPtr ground = newObject(entities["ground"]);
        ground->transform = glm::translate<float>(0, -10.1f, 0);
        scene->addChild(ground);

        meshes["suzanne_mesh"] = loadMesh("resources/suzanne.obj");
        entities["suzanne_ent"] = newEntity(materials["dull"], meshes["suzanne_mesh"]);
        ObjectPtr suzanne = newObject(entities["suzanne_ent"], scene);
        scene->addChild(suzanne);

        meshes["starMesh"] = makeStar(7, 0.3f);
        entities["star"] = newEntity(materials["dull"], meshes["starMesh"]);

        ObjectPtr left = newObject(entities["star"], scene);
        left->transform = glm::translate(-2.9f, 0.0f, 0.0f) *
                glm::scale(1.2f, 1.2f, 1.2f) *
                glm::rotate<float>(85, 0, 1, 0);
        scene->addChild(left);

        return scene;
    }


    void update() {
        root->update();
    }

private:

    void createProgram() {
        shaders["vertex"] = newVertexShader("resources/shader.vert");
        shaders["fragment"] = newFragmentShader("resources/shader.frag");

        programs["program"] = newProgram({
            shaders["vertex"],
            shaders["fragment"]
        });
    }
};

typedef std::shared_ptr<SceneManager_> SceneManagerPtr;

/*
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
*/

HackyRenderer::HackyRenderer(Root& root)
: root(root)
, clocks(root.clockManager())
, clock(clocks.getMainClock())
, scene(std::make_shared<SceneManager_>())
, cameraController(scene->camera, clock)
, counter(0)
{
    std::cout << "[Hacky] Initializing hacky renderer" << std::endl;
    scene->root = scene->createScene();

    core::registerHandler(root.dispatcher(), input::msg::INPUT_SYSTEM,
            &cameraController, &CameraController::handle);

    Renderer& renderer = root.graphics().renderer();
    using std::placeholders::_1;
    auto ratioUpdate = std::bind(&Camera::adjustRatio, &scene->camera, _1);
    renderer.viewport().listener(ratioUpdate);

    UniformPtr sunDir { new uniform3f {
        scene->sunDirection.x,
        scene->sunDirection.y,
        scene->sunDirection.z
    }};
    UniformPtr sunInt { new uniform1f { scene->sunIntensity } };
    UniformPtr ambientIntensity { new uniform1f { scene->ambient } };

    renderer.uniforms().uniform("sunDirection", sunDir);
    renderer.uniforms().uniform("sunIntensity", sunInt);
    renderer.uniforms().uniform("ambient", ambientIntensity);

    std::size_t size = 2 * sizeof(glm::mat4);
    renderer.uniforms().createUniformBlock("CameraMatrices", size);

//    MatrixRotator rotator { 30, glm::vec3 { 0, 1, 0 } };
//    taskletScheduler.add(changer(scene->root->transform, rotator));

    using std::sin;
    using std::cos;

    float maxAngle = 50.0f * M_PI / 180;


    taskletScheduler.add([=](double time, double){
        double t = time / 3;
        float angle = maxAngle * (cos(t) + 0.2);
        glm::vec3 sunPos {
            cos(angle) * cos(t),
            sin(angle),
            cos(angle) * sin(t)
        };
        scene->sunDirection = glm::normalize(-sunPos);
        return true;
    });

    /*time::Action action = repeatedly(actionScheduler, [this](){
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

    actionScheduler.scheduleIn(action, 1.0);*/
}


void HackyRenderer::updateTime() {
    ++counter;
    double time = clock.time();
    taskletScheduler.update(time, clock.dt());
    actionScheduler.update(time);
}


void drawScene(ObjectPtr obj, gfx::Renderer& renderer) {
    if (obj->entity && obj->entity->mesh) {
        renderer.submit(obj->renderable());
    }
    for (const auto& child : obj->children) {
        drawScene(child, renderer);
    }
}


void HackyRenderer::update() {
    updateTime();

    cameraController.update();
    scene->update();

    UniformPtr sunDir { new uniform3f {
        scene->sunDirection.x,
        scene->sunDirection.y,
        scene->sunDirection.z
    }};
    UniformPtr sunInt { new uniform1f { scene->sunIntensity } };
    UniformPtr ambientIntensity { new uniform1f { scene->ambient } };

    Renderer& renderer = root.graphics().renderer();
    renderer.uniforms().uniform("sunDirection", sunDir);
    renderer.uniforms().uniform("sunIntensity", sunInt);
    renderer.uniforms().uniform("ambient", ambientIntensity);

    glm::mat4 viewMatrix = scene->camera.viewMatrix();
    glm::mat4 projMatrix = scene->camera.projectionMatrix();
    std::size_t size = sizeof(glm::mat4);
    renderer.uniforms().fillUniformBlock("CameraMatrices", glm::value_ptr(viewMatrix), 0, size);
    renderer.uniforms().fillUniformBlock("CameraMatrices", glm::value_ptr(projMatrix), size, size);
    drawScene(scene->root, root.graphics().renderer());
}


} /* namespace gfx */
} /* namespace zephyr */

