/*
 * objects.h
 *
 *  Created on: Sep 8, 2013
 *      Author: los
 */

#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <GL/glew.h>
#include <GL/gl.h>
#include <zephyr/resources/ResourceManager.hpp>
#include <zephyr/gfx/Program.hpp>
#include <glm/glm.hpp>

namespace zephyr {
namespace gfx {

using namespace zephyr::resources;


typedef std::shared_ptr<struct Mesh> MeshPtr;
typedef std::shared_ptr<struct Material> MaterialPtr;
typedef std::shared_ptr<struct Entity> EntityPtr;
typedef std::shared_ptr<struct Object> ObjectPtr;
typedef std::weak_ptr<struct Object> WeakObjectPtr;

enum class Primitive {
    POINTS    = GL_POINTS,
    LINES     = GL_LINES,
    TRIANGLES = GL_TRIANGLES
};

inline GLenum primitiveToGL(Primitive primitive) {
    return static_cast<GLenum>(primitive);
}


struct Mesh: public std::enable_shared_from_this<Mesh> {
    GLuint glName;
    std::size_t count;
    bool indexed;
    GLenum indexType;
    Primitive mode;

    Mesh(GLuint glName, std::size_t count, bool indexed,
            GLenum indexType, Primitive mode = Primitive::TRIANGLES)
    : glName(glName)
    , count(count)
    , indexed(indexed)
    , indexType(indexType)
    , mode(mode)
    { }

    ~Mesh() {
        glDeleteVertexArrays(1, &glName);
    }
};


template <typename... Args>
MeshPtr newMesh(Args&&... args) {
    return std::make_shared<Mesh>(std::forward<Args>(args)...);
}

struct Material: public std::enable_shared_from_this<Material> {

    ProgramPtr program;

    explicit Material(ProgramPtr program)
    : program(program)
    { }

};


template <typename... Args>
MaterialPtr newMaterial(Args&&... args) {
    return std::make_shared<Material>(std::forward<Args>(args)...);
}


struct Entity: public std::enable_shared_from_this<Entity> {
    MaterialPtr material;
    MeshPtr buffer;

    Entity(MaterialPtr material, MeshPtr buffer)
    : material(material)
    , buffer(buffer)
    { }

};

template <typename... Args>
EntityPtr newEntity(Args&&... args) {
    return std::make_shared<Entity>(std::forward<Args>(args)...);
}


struct Object: public std::enable_shared_from_this<Object> {
    EntityPtr entity;
    WeakObjectPtr parent;
    std::vector<ObjectPtr> children;

    glm::mat4 transform;
    glm::mat4 totalTransform;


    explicit Object(EntityPtr entity, WeakObjectPtr parent = WeakObjectPtr { })
    : entity(entity)
    , parent(parent)
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
};

template <typename... Args>
ObjectPtr newObject(Args&&... args) {
    return std::make_shared<Object>(std::forward<Args>(args)...);
}

typedef ResourceManager<ShaderPtr> ShaderManager;
typedef ResourceManager<ProgramPtr> ProgramManager;
typedef ResourceManager<MaterialPtr> MaterialManager;
typedef ResourceManager<MeshPtr> VertexArrayManager;
typedef ResourceManager<EntityPtr>EntityManager;
typedef ResourceManager<ObjectPtr> ObjectManager;



} /* namespace gfx */
} /* namespace zephyr */

#endif /* OBJECTS_H_ */
