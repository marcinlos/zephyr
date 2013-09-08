/**
 * @file CameraController.hpp
 */

#ifndef ZEPHYR_GFX_CAMERACONTROLLER_HPP_
#define ZEPHYR_GFX_CAMERACONTROLLER_HPP_

#include <zephyr/input/messages.hpp>
#include <zephyr/gfx/Camera.hpp>
#include <zephyr/time/Clock.hpp>
#include <zephyr/input/InputState.hpp>
#include <zephyr/input/KeyEvent.hpp>


namespace zephyr {
namespace gfx {



class CameraController {
public:

    CameraController(Camera& camera, const time::Clock& clock)
    : camera(camera)
    , clock(clock)
    { }

    void handle(const core::Message& message) {
        using namespace zephyr::input;
        namespace events = zephyr::input::msg;

        if (message.type == events::KEYBOARD_EVENT) {
            KeyEvent e = util::any_cast<KeyEvent>(message.data);
            if (e.type == KeyEvent::Type::DOWN) {
                input[e.key] = true;

                if (e.key == Key::SPACE) {
                    std::cout << "dir: " << camera.dirFromView(FWD) << std::endl;
                    std::cout << "pos: " << camera.pos << std::endl;
                }

            } else if (e.type == KeyEvent::Type::UP) {
                input[e.key] = false;
            }
        } else if (message.type == events::BUTTON_EVENT) {
            ButtonEvent e = util::any_cast<ButtonEvent>(message.data);
            input[e.button] = (e.type == ButtonEvent::Type::DOWN);
        } else if (message.type == events::CURSOR_EVENT) {
            const float sensitivity = 0.5f;
            const float moveScale = 0.3f;
            const float rotScale = 1.0f;

            Position pos = util::any_cast<Position>(message.data);
            float dx =   sensitivity * (pos.x - input.mouse().x);
            float dy = - sensitivity * (pos.y - input.mouse().y);
            if (input[Button::RIGHT]) {
                auto viewLeft = camera.dirFromView(RIGHT);
                auto viewUp = camera.dirFromView(UP);
                camera.pos += moveScale * (dx * viewLeft + dy * viewUp);
            } else {
                float z = -100.0f;
                dx *= rotScale;
                dy *= rotScale;
                if (dx * dx + dy * dy < 500) {
                    camera.rotate(dx, dy, UP);
                }
            }
            input.mouse(pos);
        } else if (message.type == events::SCROLL_EVENT) {
            float scroll = util::any_cast<double>(message.data);
            if (input[Key::LEFT_SHIFT]) {
                Projection proj = camera.projection();
                proj.fov -= scroll;
                proj.fov = glm::clamp(proj.fov, 10.0f, 140.0f);
                camera.projection(proj);
                std::cout << "FOV: " << proj.fov << std::endl;
            } else {
                camera.pos += scroll * camera.forward();
            }
        }
    }


    void update() {
        using zephyr::input::Key;

        const float v = 4.0f;
        const float ds = v * clock.dt();

        const float hRotV = 60;
        const float hRotH = 60;

        if (input[Key::W]) {
            camera.pos += ds * camera.dirFromView(FWD);
        }
        if (input[Key::S]) {
            camera.pos += ds * camera.dirFromView(BACK);
        }
        if (input[Key::A]) {
            camera.pos += ds * camera.dirFromView(LEFT);
        }
        if (input[Key::D]) {
            camera.pos += ds * camera.dirFromView(RIGHT);
        }
        if (input[Key::E]) {
            camera.pos += ds * camera.dirFromView(UP);
        }
        if (input[Key::Q]) {
            camera.pos += ds * camera.dirFromView(DOWN);
        }

        if (input[Key::LEFT]) {
            camera.rotate(clock.dt() * -hRotH, 0, UP);
        }
        if (input[Key::RIGHT]) {
            camera.rotate(clock.dt() * hRotH, 0, UP);
        }
        if (input[Key::UP]) {
            camera.rotate(0, clock.dt() * hRotV, UP);
        }
        if (input[Key::DOWN]) {
            camera.rotate(0, clock.dt() * -hRotV, UP);
        }
    }


private:

    Camera& camera;

    const time::Clock& clock;

    input::InputState input;
};




} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_CAMERACONTROLLER_HPP_ */
