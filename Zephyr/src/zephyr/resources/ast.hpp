/**
 * @file materials.hpp
 */

#ifndef ZEPHYR_RESOURCES_AST_HPP_
#define ZEPHYR_RESOURCES_AST_HPP_

#include <zephyr/gfx/objects.h>
#include <zephyr/gfx/uniforms.hpp>
#include <zephyr/gfx/Shader.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>


namespace zephyr {
namespace resources {
namespace ast {


template <typename T>
using string_map = std::unordered_map<std::string, T>;

struct Shader {
    std::string name;
    int type;
    std::string file;
    int version;
    std::vector<std::string> defines;

    Shader(std::string name, int type, std::string file, int version = -1)
    : name { std::move(name) }
    , type { type }
    , file { std::move(file) }
    , version { version }
    { }
};

inline std::ostream& operator << (std::ostream& os, const Shader& shader) {
    os << "shader{name=" << shader.name <<
            ", type=" << gfx::shaderTypeToString(shader.type) <<
            ", file=" << shader.file <<
            ", version=" << shader.version <<
            ", defines: [";
    bool first = true;
    for (const auto& name : shader.defines) {
        os << (first ? first = false, "" : ", ") << name;
    }
    os << "]}";
    return os;
}

struct Program {
    std::string name;
    std::vector<std::string> shaders;
};

inline std::ostream& operator << (std::ostream& os, const Program& program) {
    os << "program{name=" << program.name << ", shaders=[";
    bool first = true;
    for (const auto& shader : program.shaders) {
        os << (first ? first = false, "" : ", ") << shader;
    }
    os << "]}";
    return os;
}

struct Texture {
    std::string name;
    std::string file;
};

inline std::ostream& operator << (std::ostream& os, const Texture& texture) {
    return os << "texture{name=" << texture.name << ", file=" <<
            texture.file << "}";
}


struct Material {
    std::string name;
    string_map<std::string> textures;
    string_map<gfx::UniformPtr> uniforms;
};

inline std::ostream& operator << (std::ostream& os, const Material& material) {
    os << "material{name=" << material.name << ", textures=[";
    {
        bool first = true;
        for (const auto& texPair : material.textures) {
            os << (first ? first = false, "" : ", ") << texPair.first <<
                    " : " << texPair.second;
        }
    }
    os << "], uniforms=[";
    {
        bool first = true;
        for (const auto& uniformPair : material.uniforms) {
            os << (first ? first = false, "" : ", ") << uniformPair.first <<
                    " : " << "{...}";
        }
    }
    os << "]}";
    return os;
}

struct Root {

    string_map<Shader> shaders;
    string_map<Program> programs;
    string_map<Texture> textures;
    string_map<Material> materials;

    void merge(const Root& o) {
        shaders.insert(begin(o.shaders), end(o.shaders));
        programs.insert(begin(o.programs), end(o.programs));
        textures.insert(begin(o.textures), end(o.textures));
        materials.insert(begin(o.materials), end(o.materials));
    }

};

inline std::ostream& operator << (std::ostream& os, const Root& root) {
    os << "root{\n  shaders {\n";
    for (const auto& shader : root.shaders) {
        os << "    "  << shader.second << std::endl;
    }
    os << "  }\n  programs {\n";
    for (const auto& program : root.programs) {
        os << "    " << program.second << std::endl;
    }
    os << "  }\n  textures {\n";
    for (const auto& texture : root.textures) {
        os << "    " << texture.second << std::endl;
    }
    os << "  }\n  materials {\n";
    for (const auto& material : root.materials) {
        os << "    " << material.second << std::endl;
    }
    os << "  }\n";
    os << "}";
    return os;
}

} /* namespace ast */
} /* namespace resources */
} /* namespace zephyr */

#endif /* ZEPHYR_RESOURCES_AST_HPP_ */
