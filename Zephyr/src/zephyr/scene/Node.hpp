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
        return glm::translate(position_) * glm::mat4_cast(orientation_);
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

    Node& translate(const glm::vec3& trans) {
        position_ += trans;
        return *this;
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


private:
    WeakNodePtr parent_;

    glm::vec3 position_;

    glm::quat orientation_;

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
