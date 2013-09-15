/*
 * SceneBuilder.hpp
 */

#ifndef ZEPHYR_GFX_SCENEBUILDER_HPP_
#define ZEPHYR_GFX_SCENEBUILDER_HPP_

#include <zephyr/effects/SimpleTerrainGenerator.hpp>


namespace zephyr {
namespace gfx {


struct LandscapeScene {

    struct Item {
        scene::NodePtr node;
        EntityPtr entity;
    };

    resources::ResourceSystem& res;

    std::vector<Item> items;
    scene::SceneGraph graph;

    LandscapeScene() {
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

//        NodePtr star = newNode(suzanne);
//        star->translateX(-2.9f);//.rotateX(M_PI / 2);
//        suzanne->addChild(star);

        NodePtr ministar = newNode(sceneRoot);
        sceneRoot->addChild(ministar);
        ministar->translate(1, 2, 3).scale(0.2f);

        NodePtr container = newNode(sceneRoot);
        sceneRoot->addChild(container);

        NodePtr sun = newNode(sceneRoot);
        sun->scale(0.7f, 0.7f, 0.7f);
        sceneRoot->addChild("sun", sun);

        items.push_back({ ground, res.entities["ground"] });
        items.push_back({ suzanne, res.entities["suzanne"] });
//        items.push_back({ star, entities["star"] });
        items.push_back({ container, res.entities["container"] });

        items.push_back({ ministar, res.entities["star"] });
        items.push_back({ sun, res.entities["star"] });
    }

private:
    void createResources() {
        res.shaders["vertex"] = newVertexShader("resources/shader.vert");
        res.shaders["fragment"] = newFragmentShader("resources/shader.frag");

        res.programs["program"] = newProgram({
            res.shaders["vertex"],
            res.shaders["fragment"]
        });

        res.materials["dull"] = newMaterial(res.programs["program"]);

        effects::SimpleTerrainGenerator gen(100.0f, 8, 25.0f);
        res.meshes["quad"] = gen.create();
        res.meshes["suzanne"] = loadMesh("resources/suzanne.obj");
        res.meshes["star"] = makeStar(7, 0.3f);
        res.meshes["container"] = loadMesh("resources/container.obj", NormCalc::SPLIT);


        MaterialPtr def = res.materials["dull"];
        res.entities["ground"] = newEntity(def, res.meshes["quad"]);
        res.entities["suzanne"] = newEntity(def, res.meshes["suzanne"]);
        res.entities["star"] = newEntity(def, res.meshes["star"]);
        res.entities["container"] = newEntity(def, res.meshes["container"]);
    }
};


} /* namespace gfx */
} /* namespace zephyr */


#endif /* ZEPHYR_GFX_SCENEBUILDER_HPP_ */
