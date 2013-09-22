/**
 * @file Mesh.cpp
 */

#include <zephyr/gfx/Mesh.hpp>
#include <ctime>


namespace zephyr {
namespace gfx {


MeshPtr vertexArrayFrom(const MeshData& data) {
    MeshBuilder builder;
    builder.setBuffer(data.vertices).attribute(0, 4, 0);
    if (!data.colors.empty()) {
        builder.setBuffer(data.colors).attribute(1, 4, 0);
    }
    if (!data.normals.empty()) {
        builder.setBuffer(data.normals).attribute(2, 3, 0);
    }
    if (!data.indices.empty()) {
        builder.setIndices(data.indices);
    }
    return builder.create();
}



std::vector<glm::vec4> randomColors(std::size_t count) {
    std::default_random_engine generator(std::time(nullptr));
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    std::vector<glm::vec4> colors(count * 4);
    for (auto& color : colors) {
        color.x = distribution(generator);
        color.y = distribution(generator);
        color.z = distribution(generator);
        color.w = 1.0f;
    }
    return colors;
}


MeshData loadObjData(const char* path, NormCalc strategy) {
    std::ifstream in(path);
    if (!in) {
        std::ostringstream ss("Cannot open file: ");
        ss << path;
        throw new std::runtime_error(ss.str());
    }

    std::vector<glm::vec4> vertices;
    std::vector<GLuint> indices;

    std::string line;
    while (std::getline(in, line)) {
        if (line.substr(0, 2) == "v ") {
            std::istringstream s(line.substr(2));
            glm::vec4 v;
            s >> v.x; s >> v.y; s >> v.z;
            v.w = 1.0f;
            vertices.push_back(v);
        } else if (line.substr(0, 2) == "f ") {
            std::istringstream s(line.substr(2));
            std::istream_iterator<GLushort> it(s);
            GLushort a = *it++, b = *it++, c = *it++;
            // inversion!
            indices.push_back(a - 1);
            indices.push_back(c - 1);
            indices.push_back(b - 1);
        }
    }
    MeshData data;
    if (strategy == NormCalc::SPLIT) {
        auto pair = std::tie(data.vertices, data.normals);
        pair = generateNormalsSplit(vertices, indices);
    } else {
        if (strategy == NormCalc::FIRST) {
            data.normals = generateNormalsFirst(vertices, indices);
        } else {
            data.normals = generateNormalsAvg(vertices, indices);
        }
        data.vertices = std::move(vertices);
        data.indices = std::move(indices);
    }
    return data;
}



MeshPtr loadObjMesh(const char* path, NormCalc strategy) {
    MeshData data = loadObjData(path, strategy);
    return vertexArrayFrom(data);
}

} /* namespace gfx */
} /* namespace zephyr */




