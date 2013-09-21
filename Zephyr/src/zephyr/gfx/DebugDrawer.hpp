/**
 * @file DebugDrawer.hpp
 */

#ifndef ZEPHYR_GFX_DEBUGDRAWER_HPP_
#define ZEPHYR_GFX_DEBUGDRAWER_HPP_

#include <zephyr/core/Task.hpp>
#include <zephyr/gfx/Renderer.hpp>
#include <zephyr/gfx/MeshBuilder.hpp>
#include <zephyr/gfx/objects.h>
#include <zephyr/gfx/star.hpp>
#include <zephyr/gfx/Mesh.hpp>
#include <zephyr/gfx/solids.hpp>
#include <zephyr/resources/ResourceSystem.hpp>
#include <vector>

using zephyr::core::Task;


namespace zephyr {
namespace gfx {

class DebugDrawer: public Task {
public:

    DebugDrawer(Renderer& renderer, ResourceSystem& resources)
    : renderer(renderer)
    , resources(resources)
    { init(); }


    void init() {
        mat = resources.material("debug");
        cube = makeCube();
    }

    void addBox(const glm::mat4& pos) {
        items.push_back({ newEntity(mat, cube), pos });
    }


    void update() override {
        for (const Renderable& item : items) {
            renderer.submit(item);
        }
        items.clear();
    }


private:

    MeshPtr makeCube() const {
        std::vector<glm::vec4> vertices;
        std::vector<std::uint16_t> indices;
        std::tie(vertices, indices) = makeBox<std::uint16_t>(1);

        std::vector<glm::vec4> v2;
        std::vector<glm::vec3> i2;

        std::tie(v2, i2) = generateNormalsSplit(vertices, indices);

        return MeshBuilder()
                .setBuffer(v2).attribute(0, 4)
                .setBuffer(i2).attribute(2, 3)
                .create(Primitive::LINES);
    }


    Renderer& renderer;
    ResourceSystem& resources;

    std::vector<Renderable> items;
    MeshPtr cube;

    MaterialPtr mat;
};


} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_DEBUGDRAWER_HPP_ */
