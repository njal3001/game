#include <GL/glew.h>
#include <string>
#include <unordered_map>

namespace Engine
{
    class Shader
    {
    private:
        GLuint m_id;
        std::unordered_map<std::string, GLint> m_uniform_cache;

    public:
        Shader(const std::string& vert_str, const std::string& frag_str);
        Shader(const Shader& other) = delete;
        Shader& operator=(const Shader& other) = delete;
        ~Shader();

        GLuint id() const;

        void set_uniform_1i(const GLchar* name, int value);

    private:
        GLint uniform_location(const GLchar* name);
    };
}
