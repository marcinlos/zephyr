/**
 * Object.hpp
 */

#ifndef ZEPHYR_SCENE_OBJECT_HPP_
#define ZEPHYR_SCENE_OBJECT_HPP_

#include <unordered_map>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>


namespace zephyr {
namespace scene {

typedef std::shared_ptr<class Node> NodePtr;
typedef std::weak_ptr<class Node> WeakNodePtr;


class Node: public std::enable_shared_from_this<Node> {
public:

    Node(WeakNodePtr parent = WeakNodePtr { })
    : parent_ { parent }
    { }

    void addChild(std::string name, NodePtr child) {
        children_.emplace(std::move(name), std::move(child));
    }

    void addChild(NodePtr child) {
        addChild("", child);
    }

    NodePtr child(const std::string& name) {
        auto it = children_.find(name);
        if (it != end(children_)) {
            return it->second;
        } else {
            return nullptr;
        }
    }

    NodePtr firstChild() {
        if (!children_.empty()) {
            return begin(children_)->second;
        } else {
            return nullptr;
        }
    }


    const glm::vec3& position() const {
        return position_;
    }

    glm::vec3& position() {
        return position_;
    }

    glm::quat& orientation() {
        return orientation_;
    }
    
    const glm::quat& orientation() const {
        return orientation_;
    }

    void update() {
        if (auto p = parent_.lock()) {
            transform_ = p->transform_ * localTransform();
        } else {
            transform_ = localTransform();
        }
        for (auto p : children_) {
            p.second->update();
        }
    }

    glm::mat4 localTransform() const {
        glm::mat4 translate = glm::translate(position_);
        glm::mat4 rotate = glm::mat4_cast(orientation_);
        glm::mat4 scale = glm::scale(scale_);
        return translate * rotate * scale;
    }

    glm::mat4 globalTransform() const {
        return transform_;
    }

    Node& transform(const glm::mat4& matrix) {
        glm::quat rot = glm::quat_cast(matrix);
        glm::vec3 trans { matrix[3] };
        rotate(rot);
        translate(trans);
        return *this;
    }

    Node& translateTo(const glm::vec3& position) {
        this->position_ = position;
        return *this;
    }

    Node& translateTo(float x, float y, float z) {
        return translateTo(glm::vec3 { x, y, z });
    }

    Node& translate(const glm::vec3& trans) {
        position_ += trans;
        return *this;
    }

    Node& translate(float dx, float dy, float dz) {
        return translate(glm::vec3 { dx, dy, dz });
    }

    Node& translateX(float d) {
        glm::vec3 movement { d, 0, 0 };
        return translate(movement);
    }

    Node& translateY(float d) {
        glm::vec3 movement { 0, d, 0 };
        return translate(movement);
    }

    Node& translateZ(float d) {
        glm::vec3 movement { 0, 0, d };
        return translate(movement);
    }

    Node& rotateTo(const glm::quat& orientation) {
        this->orientation_ = orientation;
        return *this;
    }

    Node& rotateTo(float pitch, float yaw, float roll) {
        glm::vec3 euler { pitch, yaw, roll };
        glm::quat rot { euler };
        return rotateTo(rot);
    }

    Node& rotate(const glm::quat& rot) {
        orientation_ = glm::cross(orientation_, rot);
        return *this;
    }

    Node& rotate(float pitch, float yaw, float roll) {
        glm::vec3 euler { pitch, yaw, roll };
        glm::quat rot { euler };
        return rotate(rot);
    }

    Node& rotateX(float radians) {
        return rotate(radians, 0, 0);
    }

    Node& rotateY(float radians) {
        return rotate(0, radians, 0);
    }

    Node& rotateZ(float radians) {
        return rotate(0, 0, radians);
    }

    Node& scale(const glm::vec3& s) {
        scale_ *= s;
        return *this;
    }

    Node& scale(float sx, float sy, float sz) {
        return scale(glm::vec3 { sx, sy, sz });
    }

    Node& scale(float s) {
        return scale(s, s, s);
    }

    Node& scaleX(float s) {
        return scale(s, 1, 1);
    }

    Node& scaleY(float s) {
        return scale(1, s, 1);
    }

    Node& scaleZ(float s) {
        return scale(1, 1, s);
    }

    Node& scaleTo(const glm::vec3& scale) {
        this->scale_ = scale;
        return *this;
    }

    Node& scaleTo(float sx, float sy, float sz) {
        return scaleTo(glm::vec3 { sx, sy, sz });
    }

private:
    WeakNodePtr parent_;

    glm::vec3 scale_ { 1, 1, 1};

    glm::quat orientation_;

    glm::vec3 position_;

    glm::mat4 transform_;

    std::unordered_multimap<std::string, NodePtr> children_;

};

template <typename... Args>
NodePtr newNode(Args&&... args) {
    return std::make_shared<Node>(std::forward<Args>(args)...);
}

} /* namespace scene */
} /* namespace zephyr */



#endif /* ZEPHYR_SCENE_OBJECT_HPP_ */
