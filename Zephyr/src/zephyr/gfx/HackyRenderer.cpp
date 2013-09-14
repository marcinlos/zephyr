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

#include <zephyr/scene/SceneGraph.hpp>


using zephyr::resources::ResourceManager;

using zephyr::effects::SimpleTerrainGenerator;


namespace zephyr {
namespace gfx {

typedef boost::io::ios_all_saver guard;


namespace scene2 {

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

struct SceneManager_ {

    ObjectPtr root;

    ShaderManager shaders;
    ProgramManager programs;
    MaterialManager materials;
    VertexArrayManager meshes;
    EntityManager entities;
    ObjectManager objects;

    void parseMaterial(const scene2::Material& material) {
        ProgramPtr program = programs[material.program];
        materials[material.name] = newMaterial(program);
    }

    void parseEntity(const scene2::Entity& entity) {
        MeshPtr mesh;
        if (! entity.mesh.empty()) {
            mesh = meshes[entity.mesh];
        }
        MaterialPtr material = materials[entity.material];
        entities[entity.name] = newEntity(material, mesh);
    }

    glm::mat4 parseTransform(const scene2::Transform& transform) const {
        const scene2::Pos& pos = transform.pos;
        const scene2::Rot& rot = transform.rot;
        const scene2::Scale& scale = transform.scale;
        return glm::translate(pos.x, pos.y, pos.z) *
                glm::scale(scale.sx, scale.sy, scale.sz) *
                glm::yawPitchRoll(rot.yaw, rot.pitch, rot.roll);
    }

    void parseNode(const scene2::Node& node, const std::string& parent) {
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

    void parseSceneDescription(const scene2::SceneDescription& scene) {
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

//    ObjectPtr createScene2() {
//        createProgram();
//        materials["dull"] = newMaterial(programs["program"]);
//        objects["root"] = newObject(nullptr);
//
//        SimpleTerrainGenerator gen(100.0f, 8, 45.0f);
//        meshes["quad"] = gen.create();
//        meshes["star 9"] = makeStar(10, 0.3f);
//
//        using namespace scene2;
//        using scene2::Entity;
//
//        Entity groundEnt { "ground", "dull", "quad" };
//        Entity starEnt { "star", "dull", "star 9" };
//
//        Node groundNode { "groundObject", "ground",
//            { Pos {0, -1, 0} }
//        };
//
//        Node starNode { "starObject", "star" };
//        groundNode.children.push_back(&starNode);
//
//        Node smallStarNode { "smallStarObject", "star", {
//                Pos { 0.9f, 0, 0},
//                Rot { },
//                Scale { 0.2f, 0.2f, 0.2f }
//            }
//        };
//        starNode.children.push_back(&smallStarNode);
//
//        Node leftStarNode { "leftStarObject", "star", {
//                { -2.9f, 0, 0 },
//                { glm::radians(85.0f), 0, 0 },
//                { 1.2f, 1.2f, 1.2f }
//            }
//        };
//        starNode.children.push_back(&leftStarNode);
//
//        scene2::SceneDescription scene {
//            { },
//            { groundEnt, starEnt },
//            { groundNode }
//        };
//
//        parseSceneDescription(scene);
//
//        return objects["root"];
//    }

    void update() {
        root->update();
    }

};

struct SceneBuilder {

    struct Item {
        scene::NodePtr node;
        EntityPtr entity;
    };

    ShaderManager shaders;
    ProgramManager programs;
    MaterialManager materials;
    VertexArrayManager meshes;
    EntityManager entities;
    ObjectManager objects;

    std::vector<Item> items;
    zephyr::scene::SceneGraph graph;

    SceneBuilder() {
        createResources();
        build();
    }


    void build() {
        using namespace zephyr::scene;
        auto& sceneRoot = graph.root();

        NodePtr ground = newNode(sceneRoot);
        ground->translateY(-10);
        sceneRoot->addChild(ground);

        NodePtr suzanne = newNode(sceneRoot);
        sceneRoot->addChild(suzanne);

        NodePtr star = newNode(suzanne);
        star->translateX(-2.9f).rotateX(M_PI / 2);
        suzanne->addChild(star);

        items.push_back({ ground, entities["ground"] });
        items.push_back({ suzanne, entities["suzanne"] });
        items.push_back({ star, entities["star"] });
    }

private:
    void createResources() {
        shaders["vertex"] = newVertexShader("resources/shader.vert");
        shaders["fragment"] = newFragmentShader("resources/shader.frag");

        programs["program"] = newProgram({
            shaders["vertex"],
            shaders["fragment"]
        });

        materials["dull"] = newMaterial(programs["program"]);

        SimpleTerrainGenerator gen(100.0f, 8, 25.0f);
        meshes["quad"] = gen.create();
        meshes["suzanne"] = loadMesh("resources/suzanne.obj");
        meshes["star"] = makeStar(7, 0.3f);

        entities["ground"] = newEntity(materials["dull"], meshes["quad"]);
        entities["suzanne"] = newEntity(materials["dull"], meshes["suzanne"]);
        entities["star"] = newEntity(materials["dull"], meshes["star"]);
    }
};


HackyRenderer::HackyRenderer(Root& root)
: root(root)
, camera {
    Projection { FOV, 1, zNear, zFar },
    glm::vec3 { 0.0f, 0.0f, 3.0f }
}
, clocks(root.clockManager())
, clock(clocks.getMainClock())
, builder(std::make_shared<SceneBuilder>())
, cameraController(camera, clock)
, sunIntensity { 1.5f }
, ambient { 0.2f }
{
    std::cout << "[Hacky] Initializing hacky renderer" << std::endl;

    core::registerHandler(root.dispatcher(), input::msg::INPUT_SYSTEM,
            &cameraController, &CameraController::handle);

    Renderer& renderer = root.graphics().renderer();
    using std::placeholders::_1;
    auto ratioUpdate = std::bind(&Camera::adjustRatio, &camera, _1);
    renderer.viewport().listener(ratioUpdate);

    UniformPtr sunDir { new uniform3f {
        sunDirection.x,
        sunDirection.y,
        sunDirection.z
    }};
    UniformPtr sunInt { new uniform1f { sunIntensity } };
    UniformPtr ambientIntensity { new uniform1f { ambient } };

    renderer.uniforms().uniform("sunDirection", sunDir);
    renderer.uniforms().uniform("sunIntensity", sunInt);
    renderer.uniforms().uniform("ambient", ambientIntensity);

    std::size_t size = 2 * sizeof(glm::mat4);
    renderer.uniforms().createUniformBlock("CameraMatrices", size);

    taskletScheduler.add([this](double t, double dt) {
        this->builder->graph.root()->firstChild()->rotateX(M_PI * dt / 5);
        return true;
    });


    float maxAngle = 50.0f * M_PI / 180;

    taskletScheduler.add([=](double time, double){
        using std::sin;
        using std::cos;
        double t = time / 3;
        float angle = maxAngle * (cos(t) + 0.2);
        glm::vec3 sunPos {
            cos(angle) * cos(t),
            sin(angle),
            cos(angle) * sin(t)
        };
        sunDirection = glm::normalize(-sunPos);
        return true;
    });
}


void HackyRenderer::updateTime() {
    double time = clock.time();
    taskletScheduler.update(time, clock.dt());
    actionScheduler.update(time);
}


void HackyRenderer::update() {
    updateTime();
    builder->graph.update();

    cameraController.update();

    UniformPtr sunDir { new uniform3f {
        sunDirection.x,
        sunDirection.y,
        sunDirection.z
    }};
    UniformPtr sunInt { new uniform1f { sunIntensity } };
    UniformPtr ambientIntensity { new uniform1f { ambient } };

    Renderer& renderer = root.graphics().renderer();
    renderer.uniforms().uniform("sunDirection", sunDir);
    renderer.uniforms().uniform("sunIntensity", sunInt);
    renderer.uniforms().uniform("ambient", ambientIntensity);

    glm::mat4 viewMatrix = camera.viewMatrix();
    glm::mat4 projMatrix = camera.projectionMatrix();
    std::size_t size = sizeof(glm::mat4);
    renderer.uniforms().fillUniformBlock("CameraMatrices", glm::value_ptr(viewMatrix), 0, size);
    renderer.uniforms().fillUniformBlock("CameraMatrices", glm::value_ptr(projMatrix), size, size);

    auto submit = [&renderer](const SceneBuilder::Item& item) {
        renderer.submit(Renderable {
            item.entity,
            item.node->globalTransform()
        });
    };
    std::for_each(begin(builder->items), end(builder->items), submit);
}


} /* namespace gfx */
} /* namespace zephyr */

