/**
 * @file materials.hpp
 */

#ifndef ZEPHYR_RESOURCES_AST_HPP_
#define ZEPHYR_RESOURCES_AST_HPP_

#include <zephyr/gfx/objects.h>
#include <zephyr/gfx/Shader.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>


namespace zephyr {
namespace resources {
namespace ast {


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


struct Root {
    std::unordered_map<std::string, Shader> shaders;
    std::unordered_map<std::string, Program> programs;

    void merge(const Root& o) {
        shaders.insert(begin(o.shaders), end(o.shaders));
        programs.insert(begin(o.programs), end(o.programs));
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
    os << "  }\n";
    os << "}";
    return os;
}

} /* namespace ast */
} /* namespace resources */
} /* namespace zephyr */

#endif /* ZEPHYR_RESOURCES_AST_HPP_ */
