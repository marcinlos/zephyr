/*
 * SceneBuilder.hpp
 */

#ifndef ZEPHYR_GFX_SCENEBUILDER_HPP_
#define ZEPHYR_GFX_SCENEBUILDER_HPP_

#include <zephyr/effects/SimpleTerrainGenerator.hpp>


namespace zephyr {
namespace gfx {


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
        star->translateX(-2.9f);//.rotateX(M_PI / 2);
        suzanne->addChild(star);

        NodePtr ministar = newNode(sceneRoot);
        sceneRoot->addChild(ministar);
        ministar->translate(1, 2, 3).scale(0.2f);

        NodePtr container = newNode(sceneRoot);
        sceneRoot->addChild(container);

        NodePtr sun = newNode(sceneRoot);
        sun->scale(0.7f, 0.7f, 0.7f);
        sceneRoot->addChild("sun", sun);

        items.push_back({ ground, entities["ground"] });
        items.push_back({ suzanne, entities["suzanne"] });
        items.push_back({ star, entities["star"] });
        items.push_back({ container, entities["container"] });

        items.push_back({ ministar, entities["star"] });
        items.push_back({ sun, entities["star"] });
    }

private:
    void createResources() {
        shaders["vertex"] = newVertexShader("resources/shader.vert");
        shaders["fragment"] = newFragmentShader("resources/shader.frag");
        shaders["vertex2"] = newVertexShader("resources/shader_flat.vert");
        shaders["fragment2"] = newFragmentShader("resources/shader_flat.frag");

        programs["program"] = newProgram({
            shaders["vertex"],
            shaders["fragment"]
        });

        programs["program2"] = newProgram({
            shaders["vertex2"],
            shaders["fragment2"]
        });

        materials["dull"] = newMaterial(programs["program"]);
        materials["flat"] = newMaterial(programs["program2"]);

        effects::SimpleTerrainGenerator gen(100.0f, 8, 25.0f);
        meshes["quad"] = gen.create();
        meshes["suzanne"] = loadMesh("resources/suzanne.obj");
        meshes["star"] = makeStar(7, 0.3f);
        meshes["container"] = loadMesh("resources/container.obj", NormCalc::SPLIT);

        entities["ground"] = newEntity(materials["flat"], meshes["quad"]);
        entities["suzanne"] = newEntity(materials["flat"], meshes["suzanne"]);
        entities["star"] = newEntity(materials["flat"], meshes["star"]);
        entities["container"] = newEntity(materials["flat"], meshes["container"]);
    }
};


} /* namespace gfx */
} /* namespace zephyr */


#endif /* ZEPHYR_GFX_SCENEBUILDER_HPP_ */
