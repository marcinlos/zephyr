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


typedef std::shared_ptr<struct VertexArray> VertexArrayPtr;
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


struct VertexArray: public std::enable_shared_from_this<VertexArray> {
    GLuint glName;
    std::size_t count;
    bool indexed;
    GLenum indexType;
    Primitive mode;

    VertexArray(GLuint glName, std::size_t count, bool indexed,
            GLenum indexType, Primitive mode = Primitive::TRIANGLES)
    : glName(glName)
    , count(count)
    , indexed(indexed)
    , indexType(indexType)
    , mode(mode)
    { }

    ~VertexArray() {
        glDeleteVertexArrays(1, &glName);
    }
};


template <typename... Args>
VertexArrayPtr newVertexArray(Args&&... args) {
    return std::make_shared<VertexArray>(std::forward<Args>(args)...);
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
    VertexArrayPtr buffer;

    Entity(MaterialPtr material, VertexArrayPtr buffer)
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
typedef ResourceManager<VertexArrayPtr> VertexArrayManager;
typedef ResourceManager<EntityPtr>EntityManager;
typedef ResourceManager<ObjectPtr> ObjectManager;


VertexArrayPtr fillVertexArray(const float* data, std::size_t n) {
    GLuint vbo;
    glGenVertexArrays(1, &vbo);
    glBindVertexArray(vbo);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n, data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(2 * n));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &buffer);

    return newVertexArray(vbo, n >> 3, false, 0);
}

template <std::size_t N>
VertexArrayPtr fillVertexArray(const float (&data)[N]) {
    return fillVertexArray(data, N);
}

VertexArrayPtr fillVertexArray(const std::vector<float>& data) {
    return fillVertexArray(&data[0], data.size());
}




}
}


#endif /* OBJECTS_H_ */
