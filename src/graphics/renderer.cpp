#include "graphics/renderer.h"
#include <assert.h>
#include "log.h"

namespace Engine
{
    Renderer::Renderer()
        :  m_vertex_map(nullptr), m_index_map(nullptr)
    {
        // Create vertex array
        glGenVertexArrays(1, &m_vertex_array);
        glBindVertexArray(m_vertex_array);

        // Create vertex buffer
        glGenBuffers(1, &m_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

        // Specify vertex buffer layout
        size_t pos_offset = 2 * sizeof(GLfloat);
        size_t uv_offset = 2 * sizeof(GLfloat);
        size_t color_offset = 4 * sizeof(GLubyte);

        GLsizei stride = pos_offset + uv_offset + color_offset;

        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)0);

        // texture uv
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)pos_offset);

        // color
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(pos_offset + uv_offset));

        // Allocate vertex buffer memory
        glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Create index buffer
        glGenBuffers(1, &m_index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
        
        // Allocate index buffer memory
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, RENDERER_INDICIES_SIZE * sizeof(GLushort), nullptr, GL_STATIC_DRAW);
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
        m_vertex_map = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
        m_index_map = (GLushort*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    }

    void Renderer::end()
    {
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        m_vertex_map = nullptr;

        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        m_index_map = nullptr;
    }

    void Renderer::make_vertex(float px, float py, float tx, float ty, Color color)
    {
        m_vertex_map->pos.x = px;
        m_vertex_map->pos.y = py;
        m_vertex_map->uv.x = tx;
        m_vertex_map->uv.y = ty;
        m_vertex_map->color = color;
        m_vertex_map++;
    }

    void Renderer::push_triangle(float px0, float py0, float px1, float py1,
             float px2, float py2, float tx0, float ty0, 
             float tx1, float ty1, float tx2, float ty2, 
             Color c0, Color c1, Color c2)
    {
        make_vertex(px0, py0, tx0, ty0, c0);
        make_vertex(px1, py1, tx1, ty1, c1);
        make_vertex(px2, py2, tx2, ty2, c2);

        *m_index_map = m_vertex_count;
        m_index_map++;
        *m_index_map = m_vertex_count + 1;
        m_index_map++;
        *m_index_map = m_vertex_count + 2;
        m_index_map++;

        m_index_count += 3;
        m_vertex_count += 3;
    }

    void Renderer::push_quad(float px0, float py0, float px1, float py1,
            float tx0, float ty0, float tx1, float ty1, 
            float tx2, float ty2, float tx3, float ty3, 
            Color c0, Color c1, Color c2, Color c3)
    {
        make_vertex(px0, py0, tx0, ty0, c0);
        make_vertex(px0, py1, tx1, ty1, c1);
        make_vertex(px1, py1, tx2, ty2, c2);
        make_vertex(px1, py0, tx3, ty3, c3);

        *m_index_map = m_vertex_count;
        m_index_map++;
        *m_index_map = m_vertex_count + 1;
        m_index_map++;
        *m_index_map = m_vertex_count + 2;
        m_index_map++;

        *m_index_map = m_vertex_count + 2;
        m_index_map++;
        *m_index_map = m_vertex_count + 3;
        m_index_map++;
        *m_index_map = m_vertex_count;

        m_index_count += 6;
        m_vertex_count += 4;
    }

    void Renderer::tri(const Vec2& pos0, const Vec2& pos1, const Vec2& pos2, const Color color)
    {
        assert(m_vertex_map && m_index_map);
        push_triangle(pos0.x, pos0.y, pos1.x, pos1.y, pos2.x, pos2.y, 0, 0, 0, 0, 0, 0, 
                color, color, color);
    }

    void Renderer::rect(const Vec2& pos, const Vec2& size, const Color color)
    {
        assert(m_vertex_map && m_index_map);
        push_quad(pos.x, pos.y, pos.x + size.x, pos.y + size.y, 
                0, 0, 0, 0, 0, 0, 0, 0, color, color, color, color);
    }

    void Renderer::tex(const Texture& texture, const Vec2& pos, const Color color)
    {
        assert(m_vertex_map && m_index_map);
    }

    void Renderer::render()
    {

    }

}
