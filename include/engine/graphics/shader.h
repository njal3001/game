#pragma once
#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include "engine/maths/mat4x4.h"

namespace Engine
{
    class Shader
    {
    private:
        GLuint m_id;
        std::unordered_map<std::string, GLint> m_uniform_cache;

    public:
        // TODO: Add constructor for file path
        Shader(const std::string& vert_str, const std::string& frag_str);
        Shader(const Shader& other) = delete;
        Shader& operator=(const Shader& other) = delete;
        ~Shader();

        GLuint id() const;

        void set_uniform_1i(const GLchar* name, int value);
        void set_uniform_mat4(const GLchar* name, const Mat4x4& value);

    private:
        GLint uniform_location(const GLchar* name);
    };
}
