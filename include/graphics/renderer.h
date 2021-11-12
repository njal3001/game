#pragma once
#include <GL/glew.h>
#include "texture.h"
#include "maths/vec2.h"
#include "color.h"

namespace Engine
{

#define RENDERER_MAX_SPRITES 1000
#define RENDERER_VERTEX_SIZE sizeof(VertexData)
#define RENDERER_BUFFER_SIZE RENDERER_MAX_SPRITES * RENDERER_VERTEX_SIZE
#define RENDERER_INDICIES_SIZE RENDERER_MAX_SPRITES * 6

    struct VertexData
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
        VertexData* m_vertex_map;

    public:
        Renderer();
        Renderer(const Renderer& other) = delete;
        Renderer& operator=(const Renderer& other) = delete;
        ~Renderer();

        void begin();
        void end();
        void render();

        void tex(const Texture& texture, const Vec2& pos, const Color color);
    };
}
