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


using zephyr::core::Message;

namespace zephyr {
namespace gfx {

using namespace zephyr::input;
namespace events = zephyr::input::msg;

class CameraController {
public:

    float linearVelocity = 4.0f;
    float horizontalAngularVelocity = 60;
    float verticalAngularVelocity = 60;


    CameraController(std::shared_ptr<Camera> camera, const time::Clock& clock)
    : camera { std::move(camera) }
    , clock(clock)
    , handlers {
        { events::KEYBOARD_EVENT, &CameraController::onKey },
        { events::BUTTON_EVENT, &CameraController::onButton },
        { events::CURSOR_EVENT, &CameraController::onMouseMove },
        { events::SCROLL_EVENT, &CameraController::onScroll }
    }
    { }

    void handle(const Message& message) {
        auto it = handlers.find(message.type);
        if (it != end(handlers)) {
            std::cout << "Processing " << message.type << std::endl;
            (this->*(it->second))(message);
        }
    }


    void update() {
        using zephyr::input::Key;
        const float ds = linearVelocity * clock.dt();

        const float hRotV = verticalAngularVelocity;
        const float hRotH = horizontalAngularVelocity;

        if (input[Key::W]) {
            moveCamera(ds, FWD);
        }
        if (input[Key::S]) {
            moveCamera(ds, BACK);
        }
        if (input[Key::A]) {
            moveCamera(ds, LEFT);
        }
        if (input[Key::D]) {
            moveCamera(ds, RIGHT);
        }
        if (input[Key::E]) {
            moveCamera(ds, UP);
        }
        if (input[Key::Q]) {
            moveCamera(ds, DOWN);
        }

        if (input[Key::LEFT]) {
            camera->rotate(clock.dt() * -hRotH, 0, UP);
        }
        if (input[Key::RIGHT]) {
            camera->rotate(clock.dt() * hRotH, 0, UP);
        }
        if (input[Key::UP]) {
            camera->rotate(0, clock.dt() * hRotV, UP);
        }
        if (input[Key::DOWN]) {
            camera->rotate(0, clock.dt() * -hRotV, UP);
        }
    }


private:

    void moveCamera(float distance, const glm::vec3& cameraSpaceDir) {
        glm::vec3 worldSpaceDir = camera->dirFromView(cameraSpaceDir);
        camera->pos += distance * worldSpaceDir;
    }

    void onScroll(const Message& message) {
        float scroll = util::any_cast<double>(message.data);
        if (input[Key::LEFT_SHIFT]) {
            Projection proj = camera->projection();
            proj.fov -= scroll;
            proj.fov = glm::clamp(proj.fov, 10.0f, 140.0f);
            camera->projection(proj);
            std::cout << "FOV: " << proj.fov << std::endl;
        } else {
            camera->pos += scroll * camera->forward();
        }
    }

    void onMouseMove(const Message& message) {
        const float sensitivity = 0.5f;
        const float moveScale = 0.3f;
        const float rotScale = 1.0f;
        Position pos = util::any_cast<Position>(message.data);
        float dx = sensitivity * (pos.x - input.mouse().x);
        float dy = -sensitivity * (pos.y - input.mouse().y);
        if (input[Button::RIGHT]) {
            auto viewLeft = camera->dirFromView(RIGHT);
            auto viewUp = camera->dirFromView(UP);
            camera->pos += moveScale * (dx * viewLeft + dy * viewUp);
        } else {
            float z = -100.0f;
            dx *= rotScale;
            dy *= rotScale;
            if (dx * dx + dy * dy < 500) {
                camera->rotate(dx, dy, UP);
            }
        }
        input.mouse(pos);
    }

    void onButton(const Message& message) {
        ButtonEvent e = util::any_cast<ButtonEvent>(message.data);
        input[e.button] = (e.type == ButtonEvent::Type::DOWN);
    }

    void onKey(const Message& message) {
        KeyEvent e = util::any_cast<KeyEvent>(message.data);
        if (e.type == KeyEvent::Type::DOWN) {
            input[e.key] = true;

            if (e.key == Key::SPACE) {
                std::cout << "dir: " << camera->dirFromView(FWD) << std::endl;
                std::cout << "pos: " << camera->pos << std::endl;
            }

        } else if (e.type == KeyEvent::Type::UP) {
            input[e.key] = false;
        }
    }

    std::shared_ptr<Camera> camera;

    const time::Clock& clock;

    input::InputState input;

    typedef void (CameraController::*Handler)(const Message&);

    std::unordered_map<std::uint32_t, Handler> handlers;

};




} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_CAMERACONTROLLER_HPP_ */
