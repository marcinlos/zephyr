/**
 * @file Mesh.cpp
 */

#include <zephyr/gfx/Mesh.hpp>
#include <boost/lexical_cast.hpp>
#include <ctime>


namespace zephyr {
namespace gfx {


MeshPtr vertexArrayFrom(const MeshData& data) {
    MeshBuilder builder;
    builder.setBuffer(data.vertices).attribute(0, 4);
    if (!data.colors.empty()) {
        builder.setBuffer(data.colors).attribute(1, 4);
    }
    if (!data.normals.empty()) {
        builder.setBuffer(data.normals).attribute(2, 3);
    }
    if (!data.uv.empty()) {
        builder.setBuffer(data.uv).attribute(3, 2);
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

struct Vertex {
    std::uint32_t vi;
    std::uint32_t ni;
    std::uint32_t ti;
};

std::uint32_t maybeRead(std::vector<std::string> words, std::size_t i) {
    using boost::lexical_cast;
    if (i < words.size()) {
        return lexical_cast<std::uint32_t>(words[i]);
    } else {
        return 0;
    }
}

void readVertex(std::istream& input, Vertex& vertex) {

    std::vector<std::string> parts;
    parts.reserve(3);

    std::string word;
    input >> word;
    std::istringstream chunk(word);

    while (getline(chunk, word, '/')) {
        parts.emplace_back(move(word));
    }
    vertex.vi = maybeRead(parts, 0);
    vertex.ti = maybeRead(parts, 1);
    vertex.ni = maybeRead(parts, 2);
}

struct ObjFileContent {
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<GLuint> indices;
};

class ObjMeshLoader {
public:

    ObjMeshLoader(std::istream& input)
    : input(input)
    { }

    ObjFileContent parse() {
        std::string line;
        while (std::getline(input, line)) {
            processLine(line);
        }
        if (! texCoords.empty()) {
            reorganizeTexCoords();
        }
        return {
            std::move(vertices),
            std::move(texCoords),
            std::move(indices)
        };
    }


private:

    void processLine(const std::string& line) {
        std::string type = line.substr(0, 2);
        if (type == "v ") {
            std::istringstream s(line.substr(2));
            glm::vec4 v;
            s >> v.x; s >> v.y; s >> v.z;
            v.w = 1.0f;
            vertices.push_back(v);
        } else if (type == "f ") {
            std::istringstream s(line.substr(2));
            Vertex a, b, c;
            readVertex(s, a);
            readVertex(s, b);
            readVertex(s, c);
            indices.push_back(a.vi - 1);
            indices.push_back(c.vi - 1);
            indices.push_back(b.vi - 1);
            texIndices.push_back(a.ti - 1);
            texIndices.push_back(c.ti - 1);
            texIndices.push_back(b.ti - 1);
        } else if (type == "vt") {
            std::istringstream s(line.substr(3));
            glm::vec2 uv;
            s >> uv.x >> uv.y;
            texCoords.push_back(uv);
        }
    }

    void reorganizeTexCoords() {
        typedef std::pair<GLuint, GLuint> index;

        auto index_hash = [](index i) -> std::size_t {
            return i.first * 31 + i.second;
        };
        typedef decltype(index_hash) hasher;
        std::size_t buckets(indices.size() * 0.75f);
        std::unordered_map<index, GLuint, hasher> pairs(buckets, index_hash);
        std::vector<glm::vec4> verts;
        std::vector<glm::vec2> uvs;
        std::vector<GLuint> ids;

        GLuint next = 0;
        for (std::uint32_t i = 0; i < indices.size(); ++ i) {
            index idx { indices[i], texIndices[i] };
            auto it = pairs.find(idx);
            if (it == end(pairs)) {
                pairs[idx] = next;
                verts.push_back(vertices[idx.first]);
                uvs.push_back(texCoords[idx.second]);
                ids.push_back(next);
                ++ next;
            } else {
                ids.push_back(it->second);
            }
        }
        vertices = std::move(verts);
        texCoords = std::move(uvs);
        indices = std::move(ids);
    }

    std::istream& input;

    std::vector<glm::vec4> vertices;
    std::vector<GLuint> indices;

    std::vector<glm::vec2> texCoords;
    std::vector<GLuint> texIndices;
};

MeshData loadObjData(const char* path, NormCalc strategy) {
    std::ifstream in(path);
    if (!in) {
        std::ostringstream ss("Cannot open file: ");
        ss << path;
        throw new std::runtime_error(ss.str());
    }
    ObjMeshLoader loader(in);
    ObjFileContent obj = loader.parse();
    MeshData data;
    if (strategy == NormCalc::SPLIT) {
        auto pair = std::tie(data.vertices, data.normals);
        pair = generateNormalsSplit(obj.vertices, obj.indices);
        if (!obj.texCoords.empty()) {
            data.uv.reserve(data.vertices.size());
            duplicate(
                begin(obj.texCoords),
                begin(obj.indices),
                end(obj.indices),
                back_inserter(data.uv));
        }
    } else {
        if (strategy == NormCalc::FIRST) {
            data.normals = generateNormalsFirst(obj.vertices, obj.indices);
        } else {
            data.normals = generateNormalsAvg(obj.vertices, obj.indices);
        }
        data.vertices = std::move(obj.vertices);
        data.indices = std::move(obj.indices);
        data.uv = std::move(obj.texCoords);
    }
    return data;
}



MeshPtr loadObjMesh(const char* path, NormCalc strategy) {
    MeshData data = loadObjData(path, strategy);
    return vertexArrayFrom(data);
}

} /* namespace gfx */
} /* namespace zephyr */




