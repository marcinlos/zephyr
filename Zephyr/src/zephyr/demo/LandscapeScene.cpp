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

void LandscapeScene::createResources() {
    using namespace gfx;
    res.loadDefinitions("resources/materials.xml");

    MaterialPtr mat = newMaterial(res.program("main-prog"));
    mat->uniforms = {
        { "diffuseColor", unif4f(1, 0.4, 0.2, 1.0f) },
        { "spec", unif1f(0.9f) }
    };

    TexturePtr texture = makeTexture();
    TexturePtr noise = makeNoise(10);

    ProgramPtr prog = res.program("main-prog");
    mat->textures = {
        { prog->uniformLocation("example"), texture },
        { prog->uniformLocation("noise"), res.texture("terrain") }
    };

    MaterialPtr terrain = newMaterial(res.program("main-prog"));
    terrain->uniforms = {
        { "diffuseColor", unif4f(0.1f, 1.0f, 0.2f, 1.0f) },
        { "spec", unif1f(0.1f) }
    };

    effects::SimpleTerrainGenerator gen(100.0f, 8, 25.0f);
    res.meshes["quad"] = gen.create();
    res.meshes["suzanne"] = loadObjMesh("resources/suzanne.obj");
    res.meshes["star"] = gfx::makeStar(7, 0.3f);
    res.meshes["container"] = loadObjMesh("resources/container.obj", NormCalc::SPLIT);


    res.entities["ground"] = newEntity(terrain, res.meshes["quad"]);
    res.entities["suzanne"] = newEntity(mat, res.meshes["suzanne"]);
    res.entities["star"] = newEntity(mat, res.meshes["star"]);
    res.entities["container"] = newEntity(mat, res.meshes["container"]);

}


} /* namespace demo */
} /* namespace zephyr */

