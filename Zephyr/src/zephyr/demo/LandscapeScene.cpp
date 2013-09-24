/**
 * @file SceneBuilder.cpp
 */

#include <zephyr/demo/LandscapeScene.hpp>
#include <zephyr/effects/SimpleTerrainGenerator.hpp>
#include <zephyr/gfx/star.hpp>
#include <zephyr/gfx/Texture.hpp>

#include <zephyr/resources/Parser.hpp>


using zephyr::effects::SimpleTerrainGenerator;

namespace zephyr {
namespace demo {



void LandscapeScene::build() {
    auto& sceneRoot = graph.root();

    NodePtr ground = newNode(sceneRoot);
    ground->translateY(-20);
    sceneRoot->addChild(ground);

    NodePtr suzanne = newNode(sceneRoot);
    sceneRoot->addChild("suzanne", suzanne);

    NodePtr cube = newNode(sceneRoot);
    sceneRoot->addChild("cube", cube);
    cube->translate(-2.8f, -1.0f, -1.7f).rotateY(0.3f);

    NodePtr ico = newNode(sceneRoot);
    sceneRoot->addChild("ico", ico);
    ico->translate(1.8f, -1.0f, -1.0f).rotateY(0.3f);

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
    items.push_back({ container, res.entities["container"] });

    items.push_back({ ministar, res.entities["star"] });
    items.push_back({ sun, res.entities["star"] });
    items.push_back({ cube, res.entities["cube"] });
    items.push_back({ ico, res.entities["ico"] });
}

void LandscapeScene::createResources() {
    using namespace gfx;
    res.loadDefinitions("resources/materials.xml");

    MaterialPtr mat = res.material("default");
    MaterialPtr terrain = res.material("terrain");
    MaterialPtr suzanne = res.material("suzanne");

    effects::SimpleTerrainGenerator gen(100.0f, 8, 25.0f);
    res.meshes["quad"] = gen.create();
    res.meshes["suzanne"] = loadObjMesh("resources/suzanne2.obj", NormCalc::SPLIT);
    res.meshes["star"] = gfx::makeStar(7, 0.3f);
    res.meshes["container"] = loadObjMesh("resources/container.obj", NormCalc::SPLIT);
    res.meshes["cube"] = loadObjMesh("resources/cube.obj", NormCalc::SPLIT);
    res.meshes["ico"] = loadObjMesh("resources/ico.obj");

    res.entities["ground"] = newEntity(terrain, res.meshes["quad"]);
    res.entities["suzanne"] = newEntity(suzanne, res.meshes["suzanne"]);
    res.entities["star"] = newEntity(res.material("white-solid"), res.meshes["star"]);
    res.entities["container"] = newEntity(mat, res.meshes["container"]);
    res.entities["cube"] = newEntity(res.material("cube"), res.meshes["cube"]);
    res.entities["ico"] = newEntity(res.material("ico"), res.meshes["ico"]);
}


} /* namespace demo */
} /* namespace zephyr */

