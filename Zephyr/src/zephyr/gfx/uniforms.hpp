/**
 * @file uniforms.hpp
 */

#ifndef UNIFORMS_HPP_
#define UNIFORMS_HPP_

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <memory>


namespace zephyr {
namespace gfx {


struct Uniform : public std::enable_shared_from_this<Uniform> {

    virtual void set(GLint location) const = 0;

    virtual ~Uniform() = default;
};

template <typename Type, std::size_t N>
class SingleUniform;



template<>
struct SingleUniform<float, 1>: Uniform {
    typedef float type;
    type value;

    explicit SingleUniform(type value)
    : value(value)
    { }

    void set(GLint location) const override {
        glUniform1f(location, value);
    }
};

template<>
struct SingleUniform<float, 2>: Uniform {
    typedef float type;
    type first;
    type second;

    explicit SingleUniform(type first, type second)
    : first(first)
    , second(second)
    { }

    void set(GLint location) const override {
        glUniform2f(location, first, second);
    }
};

template<>
struct SingleUniform<float, 3>: Uniform {
    typedef float type;
    type first;
    type second;
    type third;

    explicit SingleUniform(type first, type second, type third)
    : first(first)
    , second(second)
    , third(third)
    { }

    void set(GLint location) const override {
        glUniform3f(location, first, second, third);
    }
};


template<>
struct SingleUniform<float, 4>: Uniform {
    typedef float type;
    type first;
    type second;
    type third;
    type fourth;

    explicit SingleUniform(type first, type second, type third, type fourth)
    : first(first)
    , second(second)
    , third(third)
    , fourth(fourth)
    { }

    void set(GLint location) const override {
        glUniform4f(location, first, second, third, fourth);
    }
};

typedef SingleUniform<float, 1> uniform1f;
typedef SingleUniform<float, 2> uniform2f;
typedef SingleUniform<float, 3> uniform3f;
typedef SingleUniform<float, 4> uniform4f;







template<>
struct SingleUniform<GLint, 1>: Uniform {
    typedef GLint type;
    type value;

    explicit SingleUniform(type value)
    : value(value)
    { }

    void set(GLint location) const override {
        glUniform1i(location, value);
    }
};

template<>
struct SingleUniform<GLint, 2>: Uniform {
    typedef GLint type;
    type first;
    type second;

    explicit SingleUniform(type first, type second)
    : first(first)
    , second(second)
    { }

    void set(GLint location) const override {
        glUniform2i(location, first, second);
    }
};

template<>
struct SingleUniform<GLint, 3>: Uniform {
    typedef GLint type;
    type first;
    type second;
    type third;

    explicit SingleUniform(type first, type second, type third)
    : first(first)
    , second(second)
    , third(third)
    { }

    void set(GLint location) const override {
        glUniform3i(location, first, second, third);
    }
};


template<>
struct SingleUniform<GLint, 4>: Uniform {
    typedef GLint type;
    type first;
    type second;
    type third;
    type fourth;

    explicit SingleUniform(type first, type second, type third, type fourth)
    : first(first)
    , second(second)
    , third(third)
    , fourth(fourth)
    { }

    void set(GLint location) const override {
        glUniform4i(location, first, second, third, fourth);
    }
};

typedef SingleUniform<GLint, 1> uniform1i;
typedef SingleUniform<GLint, 2> uniform2i;
typedef SingleUniform<GLint, 3> uniform3i;
typedef SingleUniform<GLint, 4> uniform4i;




template<>
struct SingleUniform<GLuint, 1>: Uniform {
    typedef GLuint type;
    type value;

    explicit SingleUniform(type value)
    : value(value)
    { }

    void set(GLint location) const override {
        glUniform1ui(location, value);
    }
};

template<>
struct SingleUniform<GLuint, 2>: Uniform {
    typedef GLuint type;
    type first;
    type second;

    explicit SingleUniform(type first, type second)
    : first(first)
    , second(second)
    { }

    void set(GLint location) const override {
        glUniform2ui(location, first, second);
    }
};

template<>
struct SingleUniform<GLuint, 3>: Uniform {
    typedef GLuint type;
    type first;
    type second;
    type third;

    explicit SingleUniform(type first, type second, type third)
    : first(first)
    , second(second)
    , third(third)
    { }

    void set(GLint location) const override {
        glUniform3ui(location, first, second, third);
    }
};


template<>
struct SingleUniform<GLuint, 4>: Uniform {
    typedef GLuint type;
    type first;
    type second;
    type third;
    type fourth;

    explicit SingleUniform(type first, type second, type third, type fourth)
    : first(first)
    , second(second)
    , third(third)
    , fourth(fourth)
    { }

    void set(GLint location) const override {
        glUniform4ui(location, first, second, third, fourth);
    }
};

typedef SingleUniform<GLuint, 1> uniform1ui;
typedef SingleUniform<GLuint, 2> uniform2ui;
typedef SingleUniform<GLuint, 3> uniform3ui;
typedef SingleUniform<GLuint, 4> uniform4ui;



template <typename T, typename F, F Setter>
struct UniformArray: Uniform {
    GLsizei count;
    const T* data;

    void set(GLint location) const override {
        Setter(location, count, data);
    }
};


typedef UniformArray<GLfloat, decltype(&glUniform1fv), &glUniform1fv> uniform1fv;
typedef UniformArray<GLfloat, decltype(&glUniform2fv), &glUniform2fv> uniform2fv;
typedef UniformArray<GLfloat, decltype(&glUniform3fv), &glUniform3fv> uniform3fv;
typedef UniformArray<GLfloat, decltype(&glUniform4fv), &glUniform4fv> uniform4fv;

typedef UniformArray<GLfloat, decltype(&glUniform1iv), &glUniform1iv> uniform1iv;
typedef UniformArray<GLfloat, decltype(&glUniform2iv), &glUniform2iv> uniform2iv;
typedef UniformArray<GLfloat, decltype(&glUniform3iv), &glUniform3iv> uniform3iv;
typedef UniformArray<GLfloat, decltype(&glUniform4iv), &glUniform4iv> uniform4iv;

typedef UniformArray<GLfloat, decltype(&glUniform1uiv), &glUniform1uiv> uniform1uiv;
typedef UniformArray<GLfloat, decltype(&glUniform2uiv), &glUniform2uiv> uniform2uiv;
typedef UniformArray<GLfloat, decltype(&glUniform3uiv), &glUniform3uiv> uniform3uiv;
typedef UniformArray<GLfloat, decltype(&glUniform4uiv), &glUniform4uiv> uniform4uiv;


namespace detail {

    template <typename T>
    struct TypeHolder {
        typedef T type;
    };

    template <std::size_t N, std::size_t M>
    struct MatrixChooser;

    template<> struct MatrixChooser<2, 2>: TypeHolder<glm::mat2x2> { };
    template<> struct MatrixChooser<3, 3>: TypeHolder<glm::mat3x3> { };
    template<> struct MatrixChooser<4, 4>: TypeHolder<glm::mat4x4> { };
    template<> struct MatrixChooser<2, 3>: TypeHolder<glm::mat2x3> { };
    template<> struct MatrixChooser<3, 2>: TypeHolder<glm::mat3x2> { };
    template<> struct MatrixChooser<2, 4>: TypeHolder<glm::mat2x4> { };
    template<> struct MatrixChooser<4, 2>: TypeHolder<glm::mat4x2> { };
    template<> struct MatrixChooser<3, 4>: TypeHolder<glm::mat3x4> { };
    template<> struct MatrixChooser<4, 3>: TypeHolder<glm::mat4x3> { };

}

template <std::size_t N, std::size_t M, typename F, F Setter>
struct UniformMatrix: Uniform {
    typedef typename detail::MatrixChooser<N, M>::type MatrixType;
    MatrixType matrix;
    GLsizei count;
    GLboolean transponse;

    UniformMatrix(const MatrixType& matrix, GLsizei count, GLboolean transpose)
    : matrix(matrix)
    , count(count)
    , transponse(transpose)
    { }

    void set(GLint location) const override {
        (*Setter)(location, count, transponse, glm::value_ptr(matrix));
    }
};

#define MAKE_TYPE(n, m, fun, name) typedef UniformMatrix<n, m, decltype(&fun), &fun> name;

MAKE_TYPE(2, 2, glUniformMatrix2fv, uniformMatrix2f)
MAKE_TYPE(3, 3, glUniformMatrix3fv, uniformMatrix3f)
MAKE_TYPE(4, 4, glUniformMatrix4fv, uniformMatrix4f)
MAKE_TYPE(2, 3, glUniformMatrix2x3fv, uniformMatrix2x3f)
MAKE_TYPE(3, 2, glUniformMatrix3x2fv, uniformMatrix3x2f)
MAKE_TYPE(2, 4, glUniformMatrix2x4fv, uniformMatrix2x4f)
MAKE_TYPE(4, 2, glUniformMatrix4x2fv, uniformMatrix4x2f)
MAKE_TYPE(2, 4, glUniformMatrix2x4fv, uniformMatrix2x4f)
MAKE_TYPE(4, 3, glUniformMatrix4x3fv, uniformMatrix4x3f)
MAKE_TYPE(3, 4, glUniformMatrix3x4fv, uniformMatrix3x4f)

#undef MAKE_TYPE

typedef Uniform uniform;

typedef std::shared_ptr<uniform> UniformPtr;





inline UniformPtr unif1f(float v) {
    return std::make_shared<uniform1f>(v);
}

inline UniformPtr unif1f(glm::vec1 v) {
    return unif1f(v.x);
}

inline UniformPtr unif2f(float v1, float v2) {
    return std::make_shared<uniform2f>(v1, v2);
}

inline UniformPtr unif2f(const glm::vec2& v) {
    return unif2f(v.x, v.y);
}

inline UniformPtr unif3f(float v1, float v2, float v3) {
    return std::make_shared<uniform3f>(v1, v2, v3);
}

inline UniformPtr unif3f(const glm::vec3& v) {
    return unif3f(v.x, v.y, v.z);
}

inline UniformPtr unif4f(float v1, float v2, float v3, float v4) {
    return std::make_shared<uniform4f>(v1, v2, v3, v4);
}

inline UniformPtr unif4f(const glm::vec4& v) {
    return unif4f(v.x, v.y, v.z, v.w);
}

inline UniformPtr unif1i(GLint v) {
    return std::make_shared<uniform1i>(v);
}

inline UniformPtr unif2i(GLint v1, GLint v2) {
    return std::make_shared<uniform2i>(v1, v2);
}

inline UniformPtr unif3i(GLint v1, GLint v2, GLint v3) {
    return std::make_shared<uniform3i>(v1, v2, v3);
}

inline UniformPtr unif4i(GLint v1, GLint v2, GLint v3, GLint v4) {
    return std::make_shared<uniform4i>(v1, v2, v3, v4);
}

inline UniformPtr unif1ui(GLuint v) {
    return std::make_shared<uniform1ui>(v);
}

inline UniformPtr unif2ui(GLuint v1, GLuint v2) {
    return std::make_shared<uniform2ui>(v1, v2);
}

inline UniformPtr unif3ui(GLuint v1, GLuint v2, GLuint v3) {
    return std::make_shared<uniform3ui>(v1, v2, v3);
}

inline UniformPtr unif4ui(GLuint v1, GLuint v2, GLuint v3, GLuint v4) {
    return std::make_shared<uniform4ui>(v1, v2, v3, v4);
}

inline UniformPtr
unifMat2(const glm::mat2& mat, bool transpose = false, int count = 1) {
    return std::make_shared<uniformMatrix2f>(mat, count, transpose);
}

inline UniformPtr
unifMat3(const glm::mat3& mat, bool transpose = false, int count = 1) {
    return std::make_shared<uniformMatrix3f>(mat, count, transpose);
}

inline UniformPtr
unifMat4(const glm::mat4& mat, bool transpose = false, int count = 1) {
    return std::make_shared<uniformMatrix4f>(mat, count, transpose);
}

inline UniformPtr
unifMat2x3(const glm::mat2x3& mat, bool transpose = false, int count = 1) {
    return std::make_shared<uniformMatrix2x3f>(mat, count, transpose);
}

inline UniformPtr
unifMat3x2(const glm::mat3x2& mat, bool transpose = false, int count = 1) {
    return std::make_shared<uniformMatrix3x2f>(mat, count, transpose);
}

inline UniformPtr
unifMat2x4(const glm::mat2x4& mat, bool transpose = false, int count = 1) {
    return std::make_shared<uniformMatrix2x4f>(mat, count, transpose);
}

inline UniformPtr
unifMat4x2(const glm::mat4x2& mat, bool transpose = false, int count = 1) {
    return std::make_shared<uniformMatrix4x2f>(mat, count, transpose);
}

inline UniformPtr
unifMat4x3(const glm::mat4x3& mat, bool transpose = false, int count = 1) {
    return std::make_shared<uniformMatrix4x3f>(mat, count, transpose);
}

inline UniformPtr
unifMat3x4(const glm::mat3x4& mat, bool transpose = false, int count = 1) {
    return std::make_shared<uniformMatrix3x4f>(mat, count, transpose);
}



UniformPtr parse(const std::string& string);





} /* namespace gfx */
} /* namespace zephyr */




#endif /* UNIFORMS_HPP_ */
