/**
 * @file Camera.hpp
 */

#ifndef ZEPHYR_GFX_CAMERA_HPP_
#define ZEPHYR_GFX_CAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/ext.hpp>


namespace zephyr {
namespace gfx {




struct Projection {
    float fov;
    float aspectRatio;
    float zNear;
    float zFar;

    glm::mat4 matrix() const {
        return glm::perspective(fov, aspectRatio, zNear, zFar);
    }
};

const glm::vec3 ORIGIN { 0, 0, 0 };

const glm::vec3 FWD   {  0,  0, -1 };
const glm::vec3 BACK  {  0,  0,  1 };
const glm::vec3 UP    {  0,  1,  0 };
const glm::vec3 DOWN  {  0, -1,  0 };
const glm::vec3 LEFT  { -1,  0,  0 };
const glm::vec3 RIGHT {  1,  0,  0 };

class Camera {
private:
    Projection proj;
    glm::mat4 rot;

public:
    glm::vec3 pos;

    Camera(const Projection& proj, const glm::vec3& pos)
    : proj(proj)
    , pos(pos)
    { }

    glm::vec3 dirToView(const glm::vec3& v) const {
        return glm::vec3 { rot * glm::vec4(v, 0) };
    }

    glm::vec3 dirFromView(const glm::vec3& v) const {
        return glm::vec3 { glm::inverse(rot) * glm::vec4(v, 0) };
    }

    void rotate(const glm::mat4& mat) {
        rot = mat * rot;
    }

    void rotate(float dx, float dy, const glm::vec3& up) {
        glm::vec3 upInViewSpace = dirToView(up);
        glm::vec3 dir { dx, dy, -proj.zFar };
        rotate(glm::lookAt(ORIGIN, dir, upInViewSpace));
    }

    const Projection& projection() const {
        return proj;
    }

    void projection(Projection projection) {
        this->proj = projection;
    }

    glm::mat4 projectionMatrix() const {
        return proj.matrix();
    }

    glm::mat4 viewMatrix() const {
        return rot * glm::translate(-pos);
    }

    glm::vec3 up() const {
        return dirFromView(UP);
    }

    glm::vec3 forward() const {
        return dirFromView(FWD);
    }

    void adjustRatio(float aspectRatio) {
        proj.aspectRatio = aspectRatio;
    }

};


} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_CAMERA_HPP_ */
