/**
 * @file ResourceSystem.cpp
 */

#include <zephyr/resources/ResourceSystem.hpp>
#include <zephyr/resources/Parser.hpp>
#include <zephyr/gfx/Shader.hpp>

namespace zephyr {
namespace resources {

ResourceSystem::ResourceSystem(const Config& config) {
    const auto& resourceConfig = config.getNode("zephyr.resources");
    for (const auto& entry : resourceConfig) {
        const std::string& path = entry.second.data();
        loadDefinitions(path);
    }
}

void ResourceSystem::loadDefinitions(const std::string& path) {
    std::clog << "[Resources] Loading resource definitions file " <<
            path << std::endl;
    Parser parser;
    parser.parse(path);
    auto read = parser.collectAst();
    std::clog << "[Resources] Read definitions: \n" << read << std::endl;
    defs.merge(read);
}

ShaderPtr ResourceSystem::shader(const std::string& name) {
    auto val = shaders.tryGet(name);
    if (val) {
        return *val;
    } else if (ShaderPtr shader = loadShader(name)) {
        shaders.put(name, shader);
        return shader;
    } else {
        return nullptr;
    }
}

ProgramPtr ResourceSystem::program(const std::string& name) {
    auto val = programs.tryGet(name);
    if (val) {
        return *val;
    } else if (ProgramPtr program = loadProgram(name)) {
        programs.put(name, program);
        return program;
    } else {
        return nullptr;
    }
}

ShaderPtr ResourceSystem::loadShader(const std::string& name) {
    std::clog << "Loading shader " << name << std::endl;
    auto it = defs.shaders.find(name);
    if (it != end(defs.shaders)) {
        std::clog << "Found description" << std::endl;
        ast::Shader& shader = it->second;
        if (shader.version < 0) {
            shader.version = DEFAULT_SHADER_VERSION;
        }
        ShaderBuilder sb { static_cast<GLenum>(shader.type) };
        sb.version(shader.version);

        for (const std::string& symbol : shader.defines) {
            sb.define(symbol);
        }
        sb.file(shader.file);
        return sb.create();
    } else {
        std::clog << "No description found for shader " << name << std::endl;
        return nullptr;
    }
}

ProgramPtr ResourceSystem::loadProgram(const std::string& name) {
    std::clog << "Loading program " << name << std::endl;
    auto it = defs.programs.find(name);
    if (it != end(defs.programs)) {
        std::clog << "Found program definition" << std::endl;
        const ast::Program& program = it->second;
        std::vector<ShaderPtr> shaders;
        shaders.reserve(program.shaders.size());
        bool missing = false;
        for (const std::string& shaderName : program.shaders) {
            std::clog << "Linking shader " << shaderName << "..." << std::endl;
            if (const auto& shaderPtr = shader(shaderName)) {
                shaders.push_back(shaderPtr);
                std::clog << "Shader " << shaderName << " linked" << std::endl;
            } else {
                missing = true;
                std::clog << "Missing shader " << shaderName <<
                        " during linking program " << name << std::endl;
            }
        }
        return missing ? nullptr : newProgram(shaders);
    } else {
        std::clog << "No program definition found" << std::endl;
        return nullptr;
    }
}

} /* namespace resources */
} /* namespace zephyr */

