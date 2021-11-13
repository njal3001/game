#pragma once
#include <GL/glew.h>
#include "texture.h"
#include "maths/vec2.h"
#include "color.h"

namespace Engine
{

#define RENDERER_MAX_TRIANGLES 2000
#define RENDERER_VERTEX_SIZE sizeof(Vertex)
#define RENDERER_BUFFER_SIZE RENDERER_MAX_TRIANGLES * RENDERER_VERTEX_SIZE
#define RENDERER_INDICIES_SIZE RENDERER_MAX_TRIANGLES * 3

    struct Vertex
    {
        Vec2 pos;
        Vec2 uv;
        Color color;
    };

    class Renderer
    {
    private:
        GLuint m_vertex_array;
        GLuint m_vertex_buffer;
        GLuint m_index_buffer;
        Vertex* m_vertex_map;
        GLushort* m_index_map;
        GLsizei m_vertex_count;
        GLsizei m_index_count;

    public:
        Renderer();
        ~Renderer();

        Renderer(const Renderer& other) = delete;
        Renderer& operator=(const Renderer& other) = delete;

        void begin();
        void end();
        void render();

        void rect(const Vec2& pos, const Vec2& size, const Color color);
        void tri(const Vec2& pos0, const Vec2& pos1, const Vec2& pos2, const Color color); 
        void tex(const Texture& texture, const Vec2& pos, const Color color);

    private:
        void make_vertex(float px, float py, float tx, float ty, Color color);

        void push_triangle(float px0, float py0, float px1, float py1,
                 float px2, float py2, float tx0, float ty0, 
                 float tx1, float ty1, float tx2, float ty2, 
                 Color c0, Color c1, Color c2);

        void push_quad(float px0, float py0, float px1, float py1,
                float tx0, float ty0, float tx1, float ty1, 
                float tx2, float ty2, float tx3, float ty3, 
                Color c0, Color c1, Color c2, Color c3);
    };
}
