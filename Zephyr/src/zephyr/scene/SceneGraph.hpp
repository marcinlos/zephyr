/**
 * @file SceneGraph.hpp
 */

#ifndef ZEPHYR_SCENE_SCENEGRAPH_HPP_
#define ZEPHYR_SCENE_SCENEGRAPH_HPP_

#include <zephyr/scene/Node.hpp>


namespace zephyr {
namespace scene {

class SceneGraph {
public:

    SceneGraph()
    : root_ { std::make_shared<Node>() }
    { }

    const NodePtr& root() const {
        return root_;
    }


private:

    NodePtr root_;

};

} /* namespace scene */
} /* namespace zephyr */

#endif /* ZEPHYR_SCENE_SCENEGRAPH_HPP_ */
