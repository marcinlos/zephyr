/**
 * @file Lights.hpp
 */

#ifndef LIGHTS_HPP_
#define LIGHTS_HPP_

#include <zephyr/gfx/Renderer.hpp>
#include <zephyr/gfx/Camera.hpp>

using zephyr::gfx::Renderer;
using zephyr::gfx::UniformManager;
using zephyr::gfx::Camera;


namespace zephyr {
namespace demo {



class Lights {
public:

    Lights(Renderer& renderer, Camera& camera)
    : renderer(renderer)
    , uniforms(renderer.uniforms())
    , camera(camera)
    { }


    void update() {
        uniforms.set3f("lightPos", camera.pos);
        uniforms.set3f("lightAt", camera.pos + camera.forward());
    }

private:
    Renderer& renderer;
    UniformManager& uniforms;
    Camera& camera;
};

} /* namespace demo */
} /* namespace zephyr */

#endif /* LIGHTS_HPP_ */
