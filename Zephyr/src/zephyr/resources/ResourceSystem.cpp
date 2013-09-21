/**
 * @file ResourceSystem.cpp
 */

#include <zephyr/resources/ResourceSystem.hpp>
#include <zephyr/resources/Parser.hpp>
#include <zephyr/gfx/Shader.hpp>
#include <zephyr/gfx/Program.hpp>
#include <zephyr/gfx/Texture.hpp>
#include <zephyr/gfx/objects.h>

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

void notFound(const std::string& what, const std::string& name) {
    throw std::runtime_error(util::format("Cannot find {} '{}'", what, name));
}

ShaderPtr ResourceSystem::shader(const std::string& name) {
    auto val = shaders.tryGet(name);
    if (val) {
        return *val;
    } else if (ShaderPtr shader = loadShader(name)) {
        shaders.put(name, shader);
        return shader;
    } else {
        notFound("shader", name);
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
        notFound("program", name);
        return nullptr;
    }
}

TexturePtr ResourceSystem::texture(const std::string& name) {
    auto val = textures.tryGet(name);
    if (val) {
        return *val;
    } else if (TexturePtr texture = loadTexture(name)) {
        textures.put(name, texture);
        return texture;
    } else {
        notFound("texture", name);
        return nullptr;
    }
}

MaterialPtr ResourceSystem::material(const std::string& name) {
    auto val = materials.tryGet(name);
    if (val) {
        return *val;
    } else if (MaterialPtr material = loadMaterial(name)) {
        materials.put(name, material);
        return material;
    } else {
        notFound("material", name);
        return nullptr;
    }
}


ShaderPtr ResourceSystem::loadShader(const std::string& name) {
    std::clog << "Loading shader " << name << std::endl;
    auto it = defs.shaders.find(name);
    if (it != end(defs.shaders)) {
        std::clog << "Found description" << std::endl;
        ast::Shader& shaderDef = it->second;
        if (shaderDef.version < 0) {
            shaderDef.version = DEFAULT_SHADER_VERSION;
        }
        ShaderBuilder sb { static_cast<GLenum>(shaderDef.type) };
        sb.version(shaderDef.version);

        for (const auto& symbol : shaderDef.defines) {
            sb.define(symbol.first, symbol.second);
        }
        sb.file(shaderDef.file);
        ShaderPtr shader = sb.create();
        shaders.put(name, shader);
        return shader;
    } else {
        std::clog << "No description found for shader " << name << std::endl;
        return nullptr;
    }
}

ProgramPtr ResourceSystem::loadProgram(const std::string& name) {
    std::clog << "Loading program " << name << std::endl;
    auto it = defs.programs.find(name);
    if (it != end(defs.programs)) {
        std::clog << "Found programDef definition" << std::endl;
        const ast::Program& programDef = it->second;
        std::vector<ShaderPtr> shaders;
        shaders.reserve(programDef.shaders.size());
        bool missing = false;
        for (const std::string& shaderName : programDef.shaders) {
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
        if (!missing) {
            ProgramPtr program = newProgram(shaders);
            programs.put(name, program);
            return program;
        } else {
            return nullptr;
        }
    } else {
        std::clog << "No program definition for '" << name << "' found" <<
                std::endl;
        return nullptr;
    }
}

TexturePtr ResourceSystem::loadTexture(const std::string& name) {
    std::clog << "Loading texture " << name << std::endl;
    auto it = defs.textures.find(name);
    if (it != end(defs.textures)) {
        std::clog << "Found texture definition" << std::endl;
        const ast::Texture& textureDef = it->second;
        TexturePtr texture = gfx::loadTexture(textureDef.file);
        textures.put(name, texture);
        return texture;
    } else {
        std::clog << "No texture definitnion for '" << name << "' found" <<
                std::endl;
        return nullptr;
    }
}

MaterialPtr ResourceSystem::loadMaterial(const std::string& name) {
    std::clog << "Loading material " << name << std::endl;
    auto it = defs.materials.find(name);
    if (it != end(defs.materials)) {
        std::clog << "Found material definition" << std::endl;
        const ast::Material& materialDef = it->second;

        MaterialPtr material = newMaterial(program(materialDef.program));

        for (const auto& entry : materialDef.textures) {
            GLint index = material->program->uniformLocation(entry.first);
            std::clog << "Index of " << entry.first << " is " << index << std::endl;
            material->textures.push_back({ index, texture(entry.second) });
        }

        for (const auto& entry : materialDef.uniforms) {
            material->uniforms.emplace_back(entry);
        }
        materials.put(name, material);
        return material;
    } else {
        std::clog << "No material definition for '" << name << "' found" <<
                std::endl;
    return nullptr;
    }
}

} /* namespace resources */
} /* namespace zephyr */

