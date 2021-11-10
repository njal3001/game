#include "graphics/renderer.h"
#include <assert.h>

namespace Engine
{
    Renderer::Renderer()
        : m_vertex_array(0), m_vertex_buffer(0), m_index_buffer(0), m_vertex_map(nullptr)
    {
        // Create vertex array
        glGenVertexArrays(1, &m_vertex_array);
        glBindVertexArray(m_vertex_array);

        // Create vertex buffer
        glGenBuffers(1, &m_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

        // Initalize with no data, it will be filled dynamically
        glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, (const void*)NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Create index buffer
        glGenBuffers(1, &m_index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
        
        GLushort* indices = new GLushort[RENDERER_INDICIES_SIZE];

        // All vertices are indexed as quads (sprite drawing)
        unsigned int offset = 0;
        for (int i = 0; i < RENDERER_INDICIES_SIZE; i += 6)
        {
            indices[i] = offset;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset;

            offset += 4;
        }

        // Fill index buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, RENDERER_INDICIES_SIZE * sizeof(GLushort), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    Renderer::~Renderer()
    {
        glDeleteBuffers(1, &m_vertex_buffer);
        glDeleteBuffers(1, &m_index_buffer);
        glDeleteVertexArrays(1, &m_vertex_array);
    }

    void Renderer::begin()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        m_vertex_map = (VertexData*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    }

    void Renderer::end()
    {
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        m_vertex_map = nullptr;
    }

    void Renderer::render()
    {

    }

    void Renderer::tex(const Texture& texture, const Vec2& pos, const Color color)
    {
        assert(m_vertex_map);

    }
}
