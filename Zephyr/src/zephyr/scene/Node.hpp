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

    NodePtr child(const std::string& name) {
        auto it = children_.find(name);
        if (it != end(children_)) {
            return it->second;
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


private:
    WeakNodePtr parent_;

    glm::vec3 position_;

    glm::quat orientation_;

    glm::mat4 transform_;

    std::unordered_multimap<std::string, NodePtr> children_;

};

} /* namespace scene */
} /* namespace zephyr */



#endif /* ZEPHYR_SCENE_OBJECT_HPP_ */
