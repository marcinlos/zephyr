/**
 * @file ResourceSystem.hpp
 */

#ifndef ZEPHYR_RESOURCES_RESOURCESYSTEM_HPP_
#define ZEPHYR_RESOURCES_RESOURCESYSTEM_HPP_

namespace zephyr {
namespace resources{

using namespace gfx;

class ResourceSystem {
public:

    ShaderManager shaders;
    ProgramManager programs;
    MaterialManager materials;
    VertexArrayManager meshes;
    EntityManager entities;
    ObjectManager objects;

};

} /* namespace resources */
} /* namespace zephyr */

#endif /* ZEPHYR_RESOURCES_RESOURCESYSTEM_HPP_ */
