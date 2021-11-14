#pragma once
#include <GL/glew.h>
#include <unordered_map>
#include <memory>
#include "texture.h"
#include "maths/vec2.h"
#include "color.h"
#include "shader.h"

namespace Engine
{

#define RENDERER_MAX_VERTICES 10000
#define RENDERER_VERTEX_SIZE sizeof(Vertex)
#define RENDERER_BUFFER_SIZE RENDERER_MAX_VERTICES * RENDERER_VERTEX_SIZE
#define RENDERER_INDICIES_SIZE RENDERER_MAX_VERTICES


    class Renderer
    {
    private:
        struct Vertex
        {
            Vec2 pos;
            Vec2 uv;
            Color color;
        };

        GLuint m_vertex_array;
        GLuint m_vertex_buffer;
        GLuint m_index_buffer;

        Vertex m_vertices[RENDERER_MAX_VERTICES];
        GLushort m_indices[RENDERER_INDICIES_SIZE];
        unsigned int m_vert_index;
        GLushort* m_ind_p;

        // Stores texture of corresponding vertex
        int m_textures[RENDERER_MAX_VERTICES];
        // Stores the number of vertices for each texture
        std::unordered_map<int, unsigned int> m_texture_count;

        GLsizei m_vertex_count;
        GLsizei m_index_count;

        static std::shared_ptr<Shader> m_default_shader;

    public:
        Renderer();
        ~Renderer();

        Renderer(const Renderer& other) = delete;
        Renderer& operator=(const Renderer& other) = delete;

        void render();

        void rect(const Vec2& pos, const Vec2& size, const Color color);
        void tri(const Vec2& pos0, const Vec2& pos1, const Vec2& pos2, const Color color); 
        void tex(const Texture& texture, const Vec2& pos, const Color color);

    private:
        void make_vertex(float px, float py, int tex, float tx, float ty, Color color);

        void push_triangle(float px0, float py0, float px1, float py1,
                 float px2, float py2, int tex, float tx0, float ty0, 
                 float tx1, float ty1, float tx2, float ty2, 
                 Color c0, Color c1, Color c2);

        void push_quad(float px0, float py0, float px1, float py1,
                int tex, float tx0, float ty0, float tx1, float ty1, 
                float tx2, float ty2, float tx3, float ty3, 
                Color c0, Color c1, Color c2, Color c3);
    };
}
