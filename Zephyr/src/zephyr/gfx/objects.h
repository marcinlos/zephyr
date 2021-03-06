/*
 * objects.h
 *
 *  Created on: Sep 8, 2013
 *      Author: los
 */

#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <zephyr/gfx/uniforms.hpp>
#include <zephyr/resources/ResourceManager.hpp>
#include <zephyr/gfx/Program.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <unordered_map>

namespace zephyr {
namespace gfx {

using namespace zephyr::resources;


typedef std::shared_ptr<struct Mesh> MeshPtr;
typedef std::shared_ptr<struct Texture> TexturePtr;
typedef std::shared_ptr<struct Material> MaterialPtr;
typedef std::shared_ptr<struct Entity> EntityPtr;
typedef std::shared_ptr<struct Object> ObjectPtr;
typedef std::weak_ptr<struct Object> WeakObjectPtr;

enum class Primitive {
    POINTS         = GL_POINTS,
    LINES          = GL_LINES,
    LINE_STRIP     = GL_LINE_STRIP,
    LINE_LOOP      = GL_LINE_LOOP,
    TRIANGLES      = GL_TRIANGLES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN   = GL_TRIANGLE_FAN
};

inline GLenum primitiveToGL(Primitive primitive) {
    return static_cast<GLenum>(primitive);
}


struct Mesh: public std::enable_shared_from_this<Mesh> {
    GLuint id;
    std::size_t count;
    bool indexed;
    GLenum indexType;
    Primitive mode;

    Mesh(GLuint id, std::size_t count, bool indexed,
            GLenum indexType, Primitive mode = Primitive::TRIANGLES)
    : id(id)
    , count(count)
    , indexed(indexed)
    , indexType(indexType)
    , mode(mode)
    { }

    ~Mesh() {
        glDeleteVertexArrays(1, &id);
    }
};


template <typename... Args>
MeshPtr newMesh(Args&&... args) {
    return std::make_shared<Mesh>(std::forward<Args>(args)...);
}

enum struct TexDim {
    _1D = 1,
    _2D = 2,
    _3D = 3,
};

struct Texture: public std::enable_shared_from_this<Texture> {
    GLuint id;

    TexDim dim;
    int width;
    int height;
    int depth;

    Texture(GLuint id, TexDim dim, int width, int height = 1, int depth = 1)
    : id(id)
    , dim(dim)
    , width(width)
    , height(height)
    , depth(depth)
    { }

    GLuint ref() const {
        return id;
    }

    ~Texture() {
        glDeleteTextures(1, &id);
    }
};

template <typename... Args>
TexturePtr newTexture(Args&&... args) {
    return std::make_shared<Texture>(std::forward<Args>(args)...);
}

struct Material: public std::enable_shared_from_this<Material> {

    typedef std::vector<std::pair<std::string, UniformPtr>> UniformMap;
    typedef std::vector<std::pair<GLint, TexturePtr>> TextureMap;

    ProgramPtr program;
    UniformMap uniforms;
    TextureMap textures;


    explicit Material(ProgramPtr program,
            UniformMap uniforms = UniformMap { },
            TextureMap textures = TextureMap { })
    : program { std::move(program) }
    , uniforms { std::move(uniforms) }
    , textures { std::move(textures) }
    { }

};


template <typename... Args>
MaterialPtr newMaterial(Args&&... args) {
    return std::make_shared<Material>(std::forward<Args>(args)...);
}


struct Entity: public std::enable_shared_from_this<Entity> {
    MaterialPtr material;
    MeshPtr mesh;

    Entity(MaterialPtr material, MeshPtr mesh)
    : material { std::move(material) }
    , mesh { std::move(mesh) }
    { }

};

template <typename... Args>
EntityPtr newEntity(Args&&... args) {
    return std::make_shared<Entity>(std::forward<Args>(args)...);
}


struct Renderable {
    EntityPtr entity;
    glm::mat4 transform;
};


struct Object: public std::enable_shared_from_this<Object> {
    EntityPtr entity;
    WeakObjectPtr parent;
    std::vector<ObjectPtr> children;

    glm::mat4 transform;
    glm::mat4 totalTransform;


    explicit Object(EntityPtr entity, WeakObjectPtr parent = WeakObjectPtr { })
    : entity { std::move(entity) }
    , parent { std::move(parent) }
    { }

    void addChild(ObjectPtr child) {
        children.push_back(std::move(child));
    }

    void update() {
        if (auto p = parent.lock()) {
            totalTransform = p->totalTransform * transform;
        } else {
            totalTransform = transform;
        }
        for (auto child : children) {
            child->update();
        }
    }

    Renderable renderable() const {
        return { entity, transform };
    }
};

template <typename... Args>
ObjectPtr newObject(Args&&... args) {
    return std::make_shared<Object>(std::forward<Args>(args)...);
}

typedef ResourceManager<ShaderPtr> ShaderManager;
typedef ResourceManager<ProgramPtr> ProgramManager;
typedef ResourceManager<MaterialPtr> MaterialManager;
typedef ResourceManager<MeshPtr> MeshManager;
typedef ResourceManager<TexturePtr> TextureManager;
typedef ResourceManager<EntityPtr>EntityManager;
typedef ResourceManager<ObjectPtr> ObjectManager;



} /* namespace gfx */
} /* namespace zephyr */

#endif /* OBJECTS_H_ */
