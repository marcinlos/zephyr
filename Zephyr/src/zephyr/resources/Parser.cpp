/**
 * @file Parser.cpp
 */

#include <GL/glew.h>
#include <zephyr/resources/Parser.hpp>
#include <zephyr/util/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <zephyr/util/make_unique.hpp>

#include <iostream>

namespace zephyr {
namespace resources {

using boost::property_tree::ptree;
namespace flags = boost::property_tree::xml_parser;

static const int parseFlags = flags::trim_whitespace | flags::no_comments;


void Parser::parse(std::istream& input) {
    boost::property_tree::ptree ptree;
    read_xml(input, ptree, parseFlags);
    updateWith(ptree);
}

void Parser::parse(const std::string& path) {
    boost::property_tree::ptree ptree;
    read_xml(path, ptree, parseFlags);
    updateWith(ptree);
}

ast::Shader parseShader(const ptree& tree, int type) {
    const auto& attrs = tree.get_child("<xmlattr>");
    const std::string& name = attrs.get<std::string>("name");
    const std::string& file = tree.get<std::string>("file");
    int version = tree.get<int>("version", 330);

    ast::Shader shader { name, type, file, version };

    if (const auto& defines = tree.get_child_optional("defines")) {
        for (const auto& entry : *defines) {
            const std::string& val = entry.second.data();
            shader.defines.push_back(val);
        }
    }

    return shader;
}

ast::Program parseProgram(const ptree& tree) {
    const auto& attrs = tree.get_child("<xmlattr>");
    const std::string& name = attrs.get<std::string>("name");

    ast::Program program { name };

    for (const auto& child : tree) {
        if (child.first == "shader") {
            program.shaders.push_back(child.second.data());
        }
    }
    return program;
}

ast::Texture parseTexture(const ptree& tree) {
    const auto& attrs = tree.get_child("<xmlattr>");
    const std::string& name = attrs.get<std::string>("name");
    const std::string& file = tree.get<std::string>("file");

    return ast::Texture { name, file };
}

std::pair<std::string, std::string> parseTexturePair(const ptree& tree) {
    const auto& attrs = tree.get_child("<xmlattr>");
    const std::string& slot = attrs.get<std::string>("slot");
    const std::string& tex = attrs.get<std::string>("ref");
    return { slot, tex };
}

ast::Material parseMaterial(const ptree& tree) {
    const auto& attrs = tree.get_child("<xmlattr>");
    const std::string& name = attrs.get<std::string>("name");

    ast::Material material { name };

    auto texRange = tree.equal_range("texture");
    for (auto it = texRange.first; it != texRange.second; ++ it) {
        material.textures.insert(parseTexturePair(it->second));
    }



    return material;
}


void Parser::updateWith(const ptree& tree) {
    for (auto& item : tree.get_child("materials")) {
        std::string name = item.first;
        if (name == "vertex-shader") {
            ast::Shader shader = parseShader(item.second, GL_VERTEX_SHADER);
            defs.shaders.emplace(shader.name, std::move(shader));
        } else if (name == "frag-shader") {
            ast::Shader shader = parseShader(item.second, GL_FRAGMENT_SHADER);
            defs.shaders.emplace(shader.name, std::move(shader));
        } else if (name == "program") {
            ast::Program program = parseProgram(item.second);
            defs.programs.emplace(program.name, std::move(program));
        } else if (name == "texture") {
            ast::Texture texture = parseTexture(item.second);
            defs.textures.emplace(texture.name, std::move(texture));
        } else if (name == "material") {
            ast::Material material = parseMaterial(item.second);
            defs.materials.emplace(material.name, std::move(material));
        } else {
            std::clog << "[Resources] Warning: unknown entry '" << name <<
                    "'" << std::endl;
        }
    }
}

} /* namespace resources */
} /* namespace zephyr */

