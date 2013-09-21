/**
 * @file ResourceSystem.hpp
 */

#ifndef ZEPHYR_RESOURCES_RESOURCESYSTEM_HPP_
#define ZEPHYR_RESOURCES_RESOURCESYSTEM_HPP_

#include <zephyr/core/Config.hpp>
#include <zephyr/resources/ResourceManager.hpp>
#include <zephyr/gfx/objects.h>
#include <zephyr/resources/ast.hpp>

using zephyr::core::Config;

namespace zephyr {
namespace resources{

using namespace gfx;

class ResourceSystem {
public:

    ResourceSystem(const Config& config);

    ShaderManager shaders;
    ProgramManager programs;
    MaterialManager materials;
    MeshManager meshes;
    TextureManager textures;
    EntityManager entities;
    ObjectManager objects;

    void loadDefinitions(const std::string& path);

    ShaderPtr shader(const std::string& name);

    ProgramPtr program(const std::string& name);

    TexturePtr texture(const std::string& name);

    MaterialPtr material(const std::string& name);

    const unsigned int DEFAULT_SHADER_VERSION = 330;

private:

    ShaderPtr loadShader(const std::string& name);

    ProgramPtr loadProgram(const std::string& name);

    TexturePtr loadTexture(const std::string& name);

    MaterialPtr loadMaterial(const std::string& name);

    ast::Root defs;

};

} /* namespace resources */
} /* namespace zephyr */

#endif /* ZEPHYR_RESOURCES_RESOURCESYSTEM_HPP_ */
